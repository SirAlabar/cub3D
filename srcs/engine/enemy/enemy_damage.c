/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_damage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:20:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 18:45:21 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

#define ENEMY_ATTACK_DAMAGE 10
#define ENEMY_ATTACK_RANGE 1.0
#define ENEMY_ATTACK_COOLDOWN 1000

bool	enemy_can_attack(t_enemy *enemy, t_game *game)
{
	double	current_time;

	if (!enemy->alive)
		return (false);
		
	// Check if enemy is within attack range
	if (enemy->dist_to_player > ENEMY_ATTACK_RANGE)
		return (false);
		
	// Check if cooldown has passed
	current_time = get_time_ms();
	if (current_time - enemy->last_attack < ENEMY_ATTACK_COOLDOWN)
		return (false);
		
	// Check if enemy can see the player
	if (!is_enemy_visible(game, enemy->pos))
		return (false);
		
	return (true);
}

void enemy_attack_player(t_enemy *enemy, t_game *game)
{
    double current_time;

    if (!enemy_can_attack(enemy, game))
        return;
    
    // Apply damage directly to player's health
    game->p1.health -= ENEMY_ATTACK_DAMAGE;
    if (game->p1.health < 0)
        game->p1.health = 0;
    
    // Update attack timestamp
    current_time = get_time_ms();
    enemy->last_attack = current_time;
    
    // Trigger damage effect
    trigger_damage_effect(game);
    
    // Add debugging print to verify attack is happening
    printf("Enemy attacked player! Player health: %d\n", game->p1.health);
}

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
