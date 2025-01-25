/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 20:52:40 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:02:36 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	check_collisions(t_ray *ray, t_game *game)
{
	char	tile;

	tile = game->map.grid[ray->map_x][ray->map_y];
	if (tile == '1')
	{
		handle_wall_collision(ray);
		return ;
	}
	if (tile == 'D')
	{
		handle_door_collision(ray, game);
	}
}

void	handle_wall_collision(t_ray *ray)
{
	ray->hit = true;
	ray->is_door = false;
	if (ray->side == 0)
		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
}

static void	set_door_hit(t_ray *ray, double orig_dist)
{
	ray->hit = true;
	ray->is_door = true;
	ray->perp_wall_dist = orig_dist;
}

static double	get_wall_x(t_ray *ray, t_game *game, double orig_dist)
{
	double	wallx;

	if (ray->side == 0)
		wallx = game->p1.pos.y + orig_dist * ray->dir.y;
	else
		wallx = game->p1.pos.x + orig_dist * ray->dir.x;
	return (wallx - floor(wallx));
}

void	handle_door_collision(t_ray *ray, t_game *game)
{
	t_door	*door;
	double	orig_dist;
	double	wallx;

	door = find_door(game, ray->map_x, ray->map_y);
	if (!door || door->state == DOOR_OPEN)
		return ;
	if (ray->side == 0)
		orig_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		orig_dist = ray->side_dist.y - ray->delta_dist.y;
	if (door->state == DOOR_CLOSED)
	{
		set_door_hit(ray, orig_dist);
		return ;
	}
	wallx = get_wall_x(ray, game, orig_dist);
	if (wallx > door->animation)
		set_door_hit(ray, orig_dist);
}
