/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_feedback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:10:05 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 19:20:33 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

#define DAMAGE_FLASH_DURATION 200 // milliseconds
#define DAMAGE_FLASH_COLOR 0xFF0000 // Red
#define DAMAGE_FLASH_ALPHA 0.4 // 40% opacity

/**
 * Initialize damage feedback system
 * @param game Game state
 */
void	init_damage_feedback(t_game *game)
{
	game->damage_effect.active = false;
	game->damage_effect.start_time = 0;
	game->damage_effect.duration = DAMAGE_FLASH_DURATION;
}

/**
 * Trigger damage visual effect
 * @param game Game state
 */
void	trigger_damage_effect(t_game *game)
{
	game->damage_effect.active = true;
	game->damage_effect.start_time = get_time_ms();
}

/**
 * Update damage visual effect status
 * @param game Game state
 */
void	update_damage_effect(t_game *game)
{
	double current_time;

	if (!game->damage_effect.active)
		return;

	current_time = get_time_ms();
	if (current_time - game->damage_effect.start_time > game->damage_effect.duration)
	{
		game->damage_effect.active = false;
	}
}

/**
 * Draw damage visual effect (red overlay)
 * @param game Game state
 */
void	draw_damage_effect(t_game *game)
{
	int	x;
	int	y;
	int	original_color;
	int	flash_color;

	if (!game->damage_effect.active)
		return;

	// Calculate flash intensity based on time elapsed
	double elapsed = get_time_ms() - game->damage_effect.start_time;
	double intensity = 1.0 - (elapsed / game->damage_effect.duration);
	if (intensity < 0)
		intensity = 0;
	if (intensity > 1)
		intensity = 1;

	// Apply red overlay to screen
	for (y = 0; y < WINDOW_HEIGHT; y += 2) // Skip every other pixel for performance
	{
		for (x = 0; x < WINDOW_WIDTH; x += 2)
		{
			// Get existing pixel color
			char *dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
			original_color = *(unsigned int *)dst;
			
			// Blend with red
			unsigned char r = ((original_color >> 16) & 0xFF);
			unsigned char g = ((original_color >> 8) & 0xFF);
			unsigned char b = (original_color & 0xFF);
			
			// Increase red component based on intensity
			r = (unsigned char)(r + (255 - r) * DAMAGE_FLASH_ALPHA * intensity);
			g = (unsigned char)(g * (1.0 - DAMAGE_FLASH_ALPHA * intensity));
			b = (unsigned char)(b * (1.0 - DAMAGE_FLASH_ALPHA * intensity));
			
			flash_color = (0xFF << 24) | (r << 16) | (g << 8) | b;
			*(unsigned int *)dst = flash_color;
		}
	}
}

void	player_take_damage(t_game *game, int damage)
{
	game->p1.health -= damage;
	if (game->p1.health < 0)
		game->p1.health = 0;
	
	// Trigger damage visual effect
	trigger_damage_effect(game);
	
	// TODO: Add damage sound
	
	// Handle player death
	if (game->p1.health <= 0)
	{
		// TODO: Implement game over functionality
	}
}
