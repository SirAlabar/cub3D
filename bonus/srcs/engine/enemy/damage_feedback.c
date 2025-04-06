/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_feedback.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 19:10:05 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 11:36:22 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_damage_feedback(t_game *game)
{
	game->damage_effect.active = false;
	game->damage_effect.start_time = 0;
	game->damage_effect.duration = DAMAGE_FLASH_DURATION;
}

void	trigger_damage_effect(t_game *game)
{
	game->damage_effect.active = true;
	game->damage_effect.start_time = get_time_ms();
}

void	update_damage_effect(t_game *game)
{
	double	current_time;

	if (!game->damage_effect.active)
		return ;
	current_time = get_time_ms();
	if (current_time - game->damage_effect.start_time
		> game->damage_effect.duration)
	{
		game->damage_effect.active = false;
	}
}

double	calculate_flash_intensity(t_game *game)
{
	double	elapsed;
	double	intensity;

	elapsed = get_time_ms() - game->damage_effect.start_time;
	intensity = 1.0 - (elapsed / game->damage_effect.duration);
	if (intensity < 0)
		intensity = 0;
	if (intensity > 1)
		intensity = 1;
	return (intensity);
}

unsigned int	blend_color(unsigned int original, double intensity)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = ((original >> 16) & 0xFF);
	g = ((original >> 8) & 0xFF);
	b = (original & 0xFF);
	r = (unsigned char)(r + (255 - r) * DAMAGE_FLASH_ALPHA * intensity);
	g = (unsigned char)(g * (1.0 - DAMAGE_FLASH_ALPHA * intensity));
	b = (unsigned char)(b * (1.0 - DAMAGE_FLASH_ALPHA * intensity));
	return ((0xFF << 24) | (r << 16) | (g << 8) | b);
}
