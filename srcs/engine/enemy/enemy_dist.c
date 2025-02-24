/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_dist.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:14:58 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/26 17:52:44 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	calculate_enemy_distance(t_game *game, t_enemy *enemy)
{
	t_vector	dir_vector;
	double		length;

	dir_vector = vector_sub(game->p1.pos, enemy->pos);
	length = vector_length(dir_vector);
	if (length > 0.0001)
		enemy->dir = vector_div(dir_vector, length);
	else
		enemy->dir = vector_create(0, 0);
	enemy->dist_to_player = length;
}

void	update_enemy_position(t_enemy *enemy, t_game *game, double speed)
{
	t_vector	movement;
	t_vector	new_position;

	movement = vector_mult(enemy->dir, speed);
	new_position = enemy->pos;
	new_position.x += movement.x;
	if (c_enemy_mx(game, new_position, enemy->dir, 0.5))
		enemy->pos.x = new_position.x;
	new_position = enemy->pos;
	new_position.y += movement.y;
	if (c_enemy_my(game, new_position, enemy->dir, 0.5))
		enemy->pos.y = new_position.y;
}

void	update_enemies(t_game *game)
{
	t_enemy_list	*current;
	double			speed;

	current = game->enemies;
	speed = 0.03;
	while (current != NULL)
	{
		if (current->enemy.alive)
		{
			calculate_enemy_distance(game, &current->enemy);
			if (current->enemy.dist_to_player <= current->enemy.detection_radius)
			{
				update_enemy_position(&current->enemy, game, speed);
				
				// Check if enemy can attack player
				if (enemy_can_attack(&current->enemy, game))
				{
					enemy_attack_player(&current->enemy, game);
				}
			}
		}
		else
		{
			// Handle dead enemy (could add animation, despawn timer, etc.)
		}
		current = current->next;
	}
}
