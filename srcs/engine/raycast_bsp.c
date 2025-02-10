/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_bsp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:54:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/10 20:28:50 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_ray(t_ray *ray, t_game *game, int x)
{
	double	camera_x;

	mlx_sync(MLX_SYNC_IMAGE_WRITABLE, game->buffer[game->current_buffer]);
	camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->dir.x = float_to_fixed32(game->p1.dir.x + game->p1.plane.x * camera_x);
	ray->dir.y = float_to_fixed32(game->p1.dir.y + game->p1.plane.y * camera_x);
	ray->start.x = game->p1.pos.x;
	ray->start.y = game->p1.pos.y;
	ray->current_sector = find_sector(game->bsp_tree->root, ray->start);
	ray->hit = false;
	ray->buffer_index = game->current_buffer;
}

static t_fixed32	calculate_denominator(t_ray *ray, t_fixed_vec32 delta)
{
	return (fixed32_sub(
			fixed32_mul(ray->dir.x, delta.y),
			fixed32_mul(ray->dir.y, delta.x)));
}

static t_fixed32	calculate_t_value(t_ray *ray, t_bsp_line *partition,
		t_fixed_vec32 delta, t_fixed32 denom)
{
	return (fixed32_div(
			fixed32_sub(
				fixed32_mul(delta.x,
					fixed32_sub(ray->start.y, partition->start.y)),
				fixed32_mul(delta.y,
					fixed32_sub(ray->start.x, partition->start.x))),
			denom));
}

bool	check_ray_partition(t_ray *ray, t_bsp_line *partition, t_hit_info *hit)
{
	t_fixed_vec32	delta;
	t_fixed32		denom;
	t_fixed32		t;

	delta.x = fixed32_sub(partition->end.x, partition->start.x);
	delta.y = fixed32_sub(partition->end.y, partition->start.y);
	denom = calculate_denominator(ray, delta);
	if (fixed32_abs(denom) < (FIXED32_HALF >> 10))
		return (false);
	t = calculate_t_value(ray, partition, delta, denom);
	if (t < 0)
		return (false);
	hit->distance = t;
	hit->point.x = fixed32_add(ray->start.x, fixed32_mul(ray->dir.x, t));
	hit->point.y = fixed32_add(ray->start.y, fixed32_mul(ray->dir.y, t));
	hit->buffer_index = ray->buffer_index;
	return (true);
}

static void	process_partition_hit(t_ray *ray, t_bsp_node *node,
		t_game *game, t_bsp_side side)
{
	if (side == BSP_FRONT)
	{
		process_bsp_node(ray, node->front, game);
		if (!ray->hit)
			process_bsp_node(ray, node->back, game);
	}
	else
	{
		process_bsp_node(ray, node->back, game);
		if (!ray->hit)
			process_bsp_node(ray, node->front, game);
	}
}

void	process_bsp_node(t_ray *ray, t_bsp_node *node, t_game *game)
{
	t_hit_info	hit;
	t_bsp_side	side;

	if (!node)
		return ;
	if (node->is_leaf)
	{
		process_sector_walls(ray, node->sector, game);
		return ;
	}
	side = bsp_classify_point(ray->start, node->partition);
	if (check_ray_partition(ray, node->partition, &hit))
		process_partition_hit(ray, node, game, side);
	else if (side == BSP_FRONT)
		process_bsp_node(ray, node->front, game);
	else
		process_bsp_node(ray, node->back, game);
}

void	cast_rays(t_game *game, t_ray *rays)
{
	int	i;

	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		init_ray(&rays[i], game, i);
		process_bsp_node(&rays[i], game->bsp_tree->root, game);
	}
	mlx_sync(MLX_SYNC_WIN_CMD_COMPLETED, game->win);
	swap_buffers(game);
}
