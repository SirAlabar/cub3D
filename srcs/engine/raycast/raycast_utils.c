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

void	init_ray(t_ray *ray, t_game *game, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->dir.x = game->p1.dir.x + game->p1.plane.x * camera_x;
	ray->dir.y = game->p1.dir.y + game->p1.plane.y * camera_x;
	ray->map_x = (int)game->p1.pos.x;
	ray->map_y = (int)game->p1.pos.y;
	ray->delta_dist.x = fabs(1 / ray->dir.x);
	ray->delta_dist.y = fabs(1 / ray->dir.y);
	ray->hit = false;
	ray->is_door = false;
}

double	get_wall_hit_position(t_game *game, t_ray *ray)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	return (wall_x);
}

void	wall_height(t_ray *ray)
{
	ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_end >= WINDOW_HEIGHT)
		ray->draw_end = WINDOW_HEIGHT - 1;
}
