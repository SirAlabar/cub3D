/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:16:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 18:05:02 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_enemy_visible(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;
	int			max_steps;
	int			step_count;
	double		distance;

	distance = vector_dist(game->p1.pos, enemy_pos);
	if (distance < 1.5)
		return (true);
	if (!is_in_bounds(game, enemy_pos.x, enemy_pos.y))
		return (false);
	ray = init_ray_data(game, enemy_pos);
	max_steps = (int)(distance * 2) + 10;
	step_count = 0;
	while (step_count < max_steps)
	{
		step_count++;
		if ((int)ray.map_x == (int)enemy_pos.x && (int)ray.map_y == (int)enemy_pos.y)
			return (true);
		if (has_hit_wall(game, &ray))
			return (false);
		if (!is_in_bounds(game, ray.map_x, ray.map_y))
			return (false);
		if (ray.side_dist.x < ray.side_dist.y)
		{
			ray.side_dist.x += ray.delta_dist.x;
			ray.map_x += ray.step.x;
		}
		else
		{
			ray.side_dist.y += ray.delta_dist.y;
			ray.map_y += ray.step.y;
		}
	}
	return (false);
}

void	draw_enemy(t_game *game, t_enemy_list *current, double fov)
{
	t_vector	rel_pos;
	double		angle;
	double		dist;
	double		screen_x;
	int			sprite_height;
	double		effective_fov = fov;

	rel_pos = vector_sub(current->enemy.pos, game->p1.pos);
	dist = vector_length(rel_pos);
	angle = atan2(rel_pos.y, rel_pos.x) - atan2(game->p1.dir.y, game->p1.dir.x);
	while (angle < -M_PI)
		angle += 2 * M_PI;
	while (angle > M_PI)
		angle -= 2 * M_PI;
	if (dist < 2.0)
	{
		double fov_scale = 1.0 + ((2.0 - dist) / 4.0);
		effective_fov = fov * fov_scale;
	}
	screen_x = (angle + effective_fov / 2) * (WINDOW_WIDTH / effective_fov);
	sprite_height = (int)(WINDOW_HEIGHT / dist);
	if (sprite_height > WINDOW_HEIGHT * 1.5)
		sprite_height = WINDOW_HEIGHT * 1.5;
	draw_enemy_sprite(game, &current->enemy, screen_x, sprite_height);
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
		{
			if (vector_dist(game->p1.pos, current->enemy.pos) < 2.0)
			{
				draw_enemy(game, current, fov);
			}
			else if (is_enemy_visible(game, current->enemy.pos))
			{
				draw_enemy(game, current, fov);
			}
		}
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
