/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_damage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:20:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:16:28 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static bool	check_enemy_status(t_enemy *enemy)
{
	if (!enemy->alive)
		return (false);
	if (enemy->dist_to_player > ENEMY_ATTACK_RANGE)
		return (false);
	if (enemy->dist_to_player < MIN_ENEMY_DISTANCE)
		return (false);
	return (true);
}

static bool	check_attack_timing(t_enemy *enemy)
{
	double	current_time;

	current_time = get_time_ms();
	if (current_time - enemy->last_attack < ENEMY_ATTACK_COOLDOWN)
		return (false);
	return (true);
}

bool	enemy_can_attack(t_enemy *enemy, t_game *game)
{
	if (!check_enemy_status(enemy))
		return (false);
	if (!check_attack_timing(enemy))
		return (false);
	if (!is_enemy_visible(game, enemy->pos))
		return (false);
	return (true);
}

static void	apply_damage_to_player(t_enemy *enemy, t_game *game)
{
	double	current_time;

	game->p1.health -= ENEMY_ATTACK_DAMAGE;
	if (game->p1.health < 0)
		game->p1.health = 0;
	current_time = get_time_ms();
	enemy->last_attack = current_time;
	trigger_damage_effect(game);
}

void	enemy_attack_player(t_enemy *enemy, t_game *game)
{
	if (!enemy_can_attack(enemy, game))
		return ;
	apply_damage_to_player(enemy, game);
}
