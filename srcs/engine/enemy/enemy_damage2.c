/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_damage2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:47 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:05:15 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	process_enemy_attacks(t_game *game)
{
	t_enemy_list	*current;

	current = game->enemies;
	while (current != NULL)
	{
		if (current->enemy.alive)
			enemy_attack_player(&current->enemy, game);
		current = current->next;
	}
}
