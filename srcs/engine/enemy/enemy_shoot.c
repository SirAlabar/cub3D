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
    t_vector    to_enemy;
    double      angle;
    double      distance;
    double      shoot_range = 8.0;  // Reasonable shooting range
    double      angle_threshold = 0.3;  // About 17 degrees field of view for shots

    // Vector from player to enemy
    to_enemy = vector_sub(enemy->pos, game->p1.pos);
    distance = vector_length(to_enemy);
    
    // Check if enemy is within shooting range
    if (distance > shoot_range)
        return (false);

    // Calculate angle between player's direction and enemy
    angle = atan2(to_enemy.y, to_enemy.x) - 
            atan2(game->p1.dir.y, game->p1.dir.x);
    
    // Normalize angle to [-PI, PI]
    while (angle > M_PI)
        angle -= 2 * M_PI;
    while (angle < -M_PI)
        angle += 2 * M_PI;

    // Check if enemy is within our shooting angle threshold
    if (fabs(angle) > angle_threshold)
        return (false);

    // Finally, check if there's a wall between player and enemy
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
            current->enemy.health--; // Decrement health
            
            // Print hit message
            printf("Enemy at (%d, %d) hit! Health: %d\n", 
                   (int)current->enemy.pos.x, (int)current->enemy.pos.y, 
                   current->enemy.health);
            
            // Check if exactly 3 shots were taken (health = 0)
            if (current->enemy.health <= 0)
            {
                current->enemy.alive = false;
                current->enemy.health = 0; // Ensure health doesn't go negative
                printf("Enemy at (%d, %d) killed after 3rd shot!\n", 
                       (int)current->enemy.pos.x, (int)current->enemy.pos.y);
            }
            return; // Exit after hitting one enemy
        }
        current = current->next;
    }
}
