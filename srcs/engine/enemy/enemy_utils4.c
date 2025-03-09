/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:06:14 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:09:03 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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

void	check_and_draw_enemy(t_game *game, t_enemy_list *current, double fov)
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
