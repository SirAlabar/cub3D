/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:00:15 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 18:06:27 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	calculate_ray_step_x(t_ray_data *ray, t_vector ray_dir)
{
	if (ray_dir.x < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (ray->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - ray->pos.x) * ray->delta_dist.x;
	}
}

void	calculate_ray_step_y(t_ray_data *ray, t_vector ray_dir)
{
	if (ray_dir.y < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (ray->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - ray->pos.y) * ray->delta_dist.y;
	}
}

void	calculate_ray_step(t_ray_data *ray, t_vector ray_dir)
{
	calculate_ray_step_x(ray, ray_dir);
	calculate_ray_step_y(ray, ray_dir);
}

t_ray_data init_ray_data(t_game *game, t_vector enemy_pos)
{
    t_ray_data ray;
    t_vector ray_dir;
    
    // Calculate direction from player to enemy
    ray_dir = vector_sub(enemy_pos, game->p1.pos);
    
    // Normalize the direction vector
    ray_dir = vector_normalize(ray_dir);
    
    // Set up ray starting point (player position)
    ray.pos = game->p1.pos;
    ray.map_x = (int)ray.pos.x;
    ray.map_y = (int)ray.pos.y;
    
    // Handle potential division by zero
    ray.delta_dist.x = (ray_dir.x == 0) ? 1e30 : fabs(1 / ray_dir.x);
    ray.delta_dist.y = (ray_dir.y == 0) ? 1e30 : fabs(1 / ray_dir.y);
    
    // Calculate step and initial side_dist
    calculate_ray_step(&ray, ray_dir);
    
    return ray;
}

void	draw_stripe_color(t_draw_params *p)
{
	int	color;

	color = get_texture_pixel(p->enemy->texture, p->tex.x, p->tex.y);
	if ((color & 0x00FFFFFF) != 0)
	{
		draw_pixel(p->game, p->pos.x, p->pos.y,
			apply_shade(color, 1.0 / (1.0 + p->enemy->dist_to_player * 0.04)));
	}
}
