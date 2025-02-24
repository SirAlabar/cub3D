/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils.c (modified)                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 09:58:19 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 22:00:33 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_enemy_sprite(t_game *game, t_enemy *enemy, double screen_x, int sprite_height)
{
	t_sprite_data	s;
	int				stripe;

	// Set sprite dimensions
	s.height = sprite_height;
	s.width = sprite_height;
	s.screen_x = screen_x;
	
	// Calculate vertical sprite boundaries
	s.start_y = WINDOW_HEIGHT / 2 - sprite_height / 2;
	s.end_y = WINDOW_HEIGHT / 2 + sprite_height / 2;
	
	// Calculate horizontal sprite boundaries
	s.start_x = (int)(screen_x - s.width / 2);
	s.end_x = (int)(screen_x + s.width / 2);
	
	// Allow partial off-screen rendering to prevent sudden disappearance
	// Final clamp to ensure we don't try to draw outside valid memory
	if (s.start_y < 0)
		s.start_y = 0;
	if (s.end_y >= WINDOW_HEIGHT)
		s.end_y = WINDOW_HEIGHT - 1;
	if (s.start_x < 0)
		s.start_x = 0;
	if (s.end_x >= WINDOW_WIDTH)
		s.end_x = WINDOW_WIDTH - 1;
	
	// Draw the sprite
	stripe = s.start_x;
	while (stripe < s.end_x)
	{
		draw_enemy_stripe(game, enemy, &s, stripe);
		stripe++;
	}
}

// Additional helper function to determine if an enemy should be rendered
// Add this to your enemy_utils.c or a similar file
bool should_render_enemy(t_game *game, t_enemy *enemy)
{
    // Calculate angle between player direction and vector to enemy
    t_vector to_enemy = vector_sub(enemy->pos, game->p1.pos);
    t_vector player_dir = game->p1.dir;
    
    double dot_product = to_enemy.x * player_dir.x + to_enemy.y * player_dir.y;
    double magnitude_product = sqrt(to_enemy.x * to_enemy.x + to_enemy.y * to_enemy.y) * 
                              sqrt(player_dir.x * player_dir.x + player_dir.y * player_dir.y);
    
    // If enemy is very close, always render it
    if (enemy->dist_to_player < 2.0)
        return true;
        
    // Calculate cosine of angle between vectors
    double cos_angle = dot_product / magnitude_product;
    
    // Convert to angle in radians
    double angle_rad = acos(cos_angle);
    
    // If angle is within a wider FOV (~200 degrees), render the enemy
    // This makes peripheral enemies more visible
    double fov_rad = 2 * atan2(vector_mag(game->p1.plane), 1.0) * 1.2; // 120% of normal FOV
    
    return angle_rad <= fov_rad / 2;
}
