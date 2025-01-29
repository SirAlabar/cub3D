/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_shoot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 17:37:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/26 17:50:08 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_shot_hitting_enemy(t_game *game, t_enemy *enemy)
{
	t_vector	to_enemy;
	double		angle;
	double		distance;
	double		shoot_range = 8.0;
	double		angle_threshold = 0.1;

	to_enemy = vector_sub(enemy->pos, game->p1.pos);
	distance = vector_length(to_enemy);
	if (distance > shoot_range)
		return (false);
	angle = atan2(to_enemy.y, to_enemy.x) - 
			atan2(game->p1.dir.y, game->p1.dir.x);
	while (angle > M_PI)
		angle -= 2 * M_PI;
	while (angle < -M_PI)
		angle += 2 * M_PI;
	if (fabs(angle) > angle_threshold)
		return (false);
	return (is_enemy_visible(game, enemy->pos));
}

void	shoot_enemy(t_game *game)
{
	t_enemy_list *current;

	current = game->enemies;
	while (current != NULL)
	{
		if (current->enemy.alive && 
			is_shot_hitting_enemy(game, &current->enemy))
		{
			current->enemy.health--;
			printf("Enemy at (%d, %d) hit! Health: %d\n", 
				   (int)current->enemy.pos.x, (int)current->enemy.pos.y, 
				   current->enemy.health);
			if (current->enemy.health <= 0)
			{
				current->enemy.alive = false;
				current->enemy.health = 0;
			}
			return;
		}
		current = current->next;
	}
}
