/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_detection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:47:34 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:47:37 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_vector	get_wall_normal(int side, t_ray *ray)
{
	t_vector	normal;

	normal = vector_create(0, 0);
	if (side == 0)
	{
		if (ray->dir.x > 0)
			normal.x = -1;
		else
			normal.x = 1;
	}
	else
	{
		if (ray->dir.y > 0)
			normal.y = -1;
		else
			normal.y = 1;
	}
	return (normal);
}

static void	get_wall_info(t_ray *ray, t_game *game, t_portal_hit *hit)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = game->p1.pos.y + ((ray->map_x - game->p1.pos.x + (1
						- ray->step_x) / 2) / ray->dir.x) * ray->dir.y;
	else
		wall_x = game->p1.pos.x + ((ray->map_y - game->p1.pos.y + (1
						- ray->step_y) / 2) / ray->dir.y) * ray->dir.x;
	wall_x -= floor(wall_x);
	hit->found = true;
	hit->wall_pos = vector_i_create(ray->map_x, ray->map_y);
	hit->normal = get_wall_normal(ray->side, ray);
	hit->offset = wall_x;
	if (ray->side == 0)
		hit->distance = (ray->map_x - game->p1.pos.x + (1 - ray->step_x) / 2)
			/ ray->dir.x;
	else
		hit->distance = (ray->map_y - game->p1.pos.y + (1 - ray->step_y) / 2)
			/ ray->dir.y;
	hit->hit_pos.x = game->p1.pos.x + ray->dir.x * hit->distance;
	hit->hit_pos.y = game->p1.pos.y + ray->dir.y * hit->distance;
}

static void	process_ray_step(t_ray *ray)
{
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

t_portal_hit	detect_wall_for_portal(t_game *game)
{
	t_ray			ray;
	t_portal_hit	hit;
	int				max_dist;

	init_hit(&hit);
	init_ray(&ray, game, WINDOW_WIDTH >> 1);
	step_side_dist(&ray, game);
	max_dist = 10;
	while (max_dist > 0 && !hit.found)
	{
		process_ray_step(&ray);
		if (!is_within_map_bounds(game, ray.map_x, ray.map_y))
			break ;
		if (game->map.grid[ray.map_x][ray.map_y] == '1')
		{
			get_wall_info(&ray, game, &hit);
		}
		max_dist--;
	}
	return (hit);
}

bool	is_valid_portal_surface(t_game *game, t_portal_hit hit)
{
	char	tile;

	if (!hit.found || hit.distance > 8.0)
		return (false);
	tile = game->map.grid[hit.wall_pos.x][hit.wall_pos.y];
	if (tile != '1')
		return (false);
	return (true);
}
