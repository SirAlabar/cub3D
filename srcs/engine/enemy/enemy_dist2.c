/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:50:35 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 11:50:59 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	process_active_enemy(t_enemy_list *current, t_game *game,
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
