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
    t_vector shot_pos = game->p1.pos; // Use player position as shot origin
    double   distance = vector_dist(shot_pos, enemy->pos);

    // If the shot is close enough to the enemy, it's a hit
    if (distance < 0.5) // HIT_RANGE can be a small value like 0.5
        return (true);
    return (false);
}

void	shoot_enemy(t_game *game)
{
    t_enemy_list *current;

    current = game->enemies; // Start at the head of the linked list
    while (current != NULL)
    {
        if (current->enemy.alive && 
            is_shot_hitting_enemy(game, &current->enemy)) // Check if shot hits enemy
        {
            current->enemy.health--; // Decrement health
            if (current->enemy.health <= 0) // Check if enemy should die
            {
                current->enemy.alive = false; // Mark enemy as dead
                printf("Enemy at (%d, %d) has been killed!\n", 
                       (int)current->enemy.pos.x, (int)current->enemy.pos.y);
            }
            else
            {
                printf("Enemy at (%d, %d) has been hit! Health remaining: %d\n", 
                       (int)current->enemy.pos.x, (int)current->enemy.pos.y, 
                       current->enemy.health);
            }
            break; // Exit loop after hitting one enemy
        }
        current = current->next; // Move to the next enemy in the list
    }
}
