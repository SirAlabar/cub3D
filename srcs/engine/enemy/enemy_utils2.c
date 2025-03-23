/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:16:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:09:04 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	check_short_distance(t_game *game, t_vector enemy_pos)
{
	double	distance;

	distance = vector_dist(game->p1.pos, enemy_pos);
	if (distance < 1.5)
		return (true);
	if (!is_in_bounds(game, enemy_pos.x, enemy_pos.y))
		return (false);
	return (false);
}

bool	ray_trace_to_enemy(t_game *game, t_ray_data *ray,
	t_vector enemy_pos, int max_steps)
{
	int	step_count;

	step_count = 0;
	while (step_count < max_steps)
	{
		step_count++;
		if ((int)ray->map_x == (int)enemy_pos.x
			&& (int)ray->map_y == (int)enemy_pos.y)
			return (true);
		if (has_hit_wall(game, ray))
			return (false);
		if (!is_in_bounds(game, ray->map_x, ray->map_y))
			return (false);
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step.x;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step.y;
		}
	}
	return (true);
}

bool	is_enemy_visible(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;
	int			max_steps;
	double		distance;

	distance = vector_dist(game->p1.pos, enemy_pos);
	if (distance < 2.0)
		return (true);
	ray = init_ray_data(game, enemy_pos);
	max_steps = (int)(distance * 4) + 20;
	return (ray_trace_to_enemy(game, &ray, enemy_pos, max_steps));
}

void	adjust_angle_and_fov(double *angle, double *effective_fov,
	double fov, double dist)
{
	double	fov_scale;

	while (*angle < -M_PI)
		*angle += 2 * M_PI;
	while (*angle > M_PI)
		*angle -= 2 * M_PI;
	*effective_fov = fov;
	if (dist < 2.0)
	{
		fov_scale = 1.0 + ((2.0 - dist) / 4.0);
		*effective_fov = fov * fov_scale;
	}
}

int	calculate_sprite_height(double dist)
{
	int	sprite_height;

	sprite_height = (int)(WINDOW_HEIGHT / dist);
	if (sprite_height > WINDOW_HEIGHT * 1.5)
		sprite_height = WINDOW_HEIGHT * 1.5;
	return (sprite_height);
}
