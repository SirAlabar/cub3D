/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_dist.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:14:58 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 11:49:17 by marsoare         ###   ########.fr       */
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

static t_vector	try_move_x(t_enemy *enemy, t_game *game,
	double speed)
{
	t_vector	movement;
	t_vector	new_position;

	movement = vector_mult(enemy->dir, speed);
	new_position = enemy->pos;
	new_position.x += movement.x;
	if (c_enemy_mx(game, new_position, enemy->dir, 0.5))
		enemy->pos.x = new_position.x;
	return (enemy->pos);
}

static t_vector	try_move_y(t_enemy *enemy, t_game *game,
	double speed)
{
	t_vector	movement;
	t_vector	new_position;

	movement = vector_mult(enemy->dir, speed);
	new_position = enemy->pos;
	new_position.y += movement.y;
	if (c_enemy_my(game, new_position, enemy->dir, 0.5))
		enemy->pos.y = new_position.y;
	return (enemy->pos);
}

void	update_enemy_position(t_enemy *enemy, t_game *game, double speed)
{
	if (enemy->dist_to_player <= MIN_ENEMY_DISTANCE)
		return ;
	enemy->pos = try_move_x(enemy, game, speed);
	enemy->pos = try_move_y(enemy, game, speed);
}

static void	process_active_enemy(t_enemy_list *current, t_game *game, 
	double speed)
{
	calculate_enemy_distance(game, &current->enemy);
	if (current->enemy.dist_to_player <= current->enemy.detection_radius)
	{
		update_enemy_position(&current->enemy, game, speed);
		if (enemy_can_attack(&current->enemy, game))
			enemy_attack_player(&current->enemy, game);
	}
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
			process_active_enemy(current, game, speed);
		current = current->next;
	}
}
