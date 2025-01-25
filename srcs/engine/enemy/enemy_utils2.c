/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:16:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/25 10:21:33 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_enemy_visible(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;

	ray = init_ray_data(game, enemy_pos);
	while (1)
	{
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
		if (ray.map_x == (int)enemy_pos.x && ray.map_y == (int)enemy_pos.y)
			return (true);
		if (game->map.grid[ray.map_x][ray.map_y] == '1')
			return (false);
	}
}

void	draw_enemy(t_game *game, t_enemy_list *current, double fov)
{
	t_vector	rel_pos;
	double		angle;
	double		dist;

	rel_pos = vector_sub(current->enemy.pos, game->p1.pos);
	dist = vector_dist(game->p1.pos, current->enemy.pos);
	angle = atan2(rel_pos.y, rel_pos.x)
		- atan2(game->p1.dir.y, game->p1.dir.x);
	if (angle < -M_PI)
		angle += 2 * M_PI;
	if (angle > M_PI)
		angle -= 2 * M_PI;
	draw_enemy_sprite(game, &current->enemy, (angle + fov / 2)
		* (WINDOW_WIDTH / fov), (int)(WINDOW_HEIGHT / dist));
}

void	draw_enemies(t_game *game)
{
	t_enemy_list	*current;
	double			fov;

	current = game->enemies;
	fov = 2 * atan2(vector_length(game->p1.plane), 1.0);
	while (current && current->enemy.alive)
	{
		if (!is_enemy_visible(game, current->enemy.pos))
		{
			current = current->next;
			continue ;
		}
		draw_enemy(game, current, fov);
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
			if (game->map.grid[i][j] == 'E')
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
