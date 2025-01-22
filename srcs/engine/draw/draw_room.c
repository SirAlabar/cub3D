/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:09:52 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	draw_background(t_game *game)
{
	int	x;
	int	y;

	if (!game || !game->img)
	{
		ft_printf("Error: Game or image pointer is NULL\n");
		return (0);
	}
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		x = -1;
		while (++x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				draw_pixel(game, x, y, game->map.ceiling_color);
			else
				draw_pixel(game, x, y, game->map.floor_color);
		}
	}
	return (1);
}

void	update_ray_position(t_ray *ray)
{
	if (ray->side == 0)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->map_x += ray->step_x;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->map_y += ray->step_y;
	}
	ray->hit = false;
	ray->is_door = false;
}

void	get_hit_position(t_ray *ray, t_game *game, double orig_dist,
		double *door_hit_pos)
{
	t_vector	hit_pos;

	hit_pos = vector_create(ray->dir.x, ray->dir.y);
	hit_pos = vector_mult(hit_pos, orig_dist);
	hit_pos = vector_add(vector_create(game->p1.pos.x, game->p1.pos.y),
			hit_pos);
	if (ray->side == 0)
		*door_hit_pos = hit_pos.y;
	else
		*door_hit_pos = hit_pos.x;
	*door_hit_pos -= floor(*door_hit_pos);
}
