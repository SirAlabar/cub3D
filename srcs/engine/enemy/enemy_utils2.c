/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:16:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 11:55:39 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static bool	check_short_distance(t_game *game, t_vector enemy_pos)
{
	double	distance;

	distance = vector_dist(game->p1.pos, enemy_pos);
	if (distance < 1.5)
		return (true);
	if (!is_in_bounds(game, enemy_pos.x, enemy_pos.y))
		return (false);
	return (false);
}

static bool	ray_trace_to_enemy(t_game *game, t_ray_data *ray,
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
	return (false);
}

bool	is_enemy_visible(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;
	int			max_steps;
	double		distance;

	if (check_short_distance(game, enemy_pos))
		return (true);
	distance = vector_dist(game->p1.pos, enemy_pos);
	ray = init_ray_data(game, enemy_pos);
	max_steps = (int)(distance * 2) + 10;
	return (ray_trace_to_enemy(game, &ray, enemy_pos, max_steps));
}

static void	adjust_angle_and_fov(double *angle, double *effective_fov,
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

static int	calculate_sprite_height(double dist)
{
	int	sprite_height;

	sprite_height = (int)(WINDOW_HEIGHT / dist);
	if (sprite_height > WINDOW_HEIGHT * 1.5)
		sprite_height = WINDOW_HEIGHT * 1.5;
	return (sprite_height);
}

void	draw_enemy(t_game *game, t_enemy_list *current, double fov)
{
	t_vector	rel_pos;
	double		angle;
	double		dist;
	double		screen_x;
	double		effective_fov;

	rel_pos = vector_sub(current->enemy.pos, game->p1.pos);
	dist = vector_length(rel_pos);
	angle = atan2(rel_pos.y, rel_pos.x)
		- atan2(game->p1.dir.y, game->p1.dir.x);
	adjust_angle_and_fov(&angle, &effective_fov, fov, dist);
	screen_x = (angle + effective_fov / 2) * (WINDOW_WIDTH / effective_fov);
	draw_enemy_sprite(game, &current->enemy, screen_x,
		calculate_sprite_height(dist));
}

static void	check_and_draw_enemy(t_game *game, t_enemy_list *current, double fov)
{
	if (vector_dist(game->p1.pos, current->enemy.pos) < 2.0)
		draw_enemy(game, current, fov);
	else if (is_enemy_visible(game, current->enemy.pos))
		draw_enemy(game, current, fov);
}

void	draw_enemies(t_game *game)
{
	t_enemy_list	*current;
	double			fov;

	current = game->enemies;
	fov = 2 * atan2(vector_mag(game->p1.plane), 1.0);
	while (current != NULL)
	{
		if (current->enemy.alive)
			check_and_draw_enemy(game, current, fov);
		current = current->next;
	}
}

void	spawn_enemies_from_map(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'M')
			{
				add_enemy(game, vector_create(j + 0.5, i + 0.5));
				game->map.grid[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}

void	init_enemies(t_game *game)
{
	game->enemies = NULL;
	spawn_enemies_from_map(game);
}
