/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_feedback2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:59:00 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:02:27 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	apply_damage_overlay(t_game *game, double intensity)
{
	int				x;
	int				y;
	char			*dst;
	unsigned int	color;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			dst = game->addr + (y * game->line_length
					+ x * (game->bits_per_pixel / 8));
			color = *(unsigned int *)dst;
			*(unsigned int *)dst = blend_color(color, intensity);
			x += 2;
		}
		y += 2;
	}
}

void	draw_damage_effect(t_game *game)
{
	double	intensity;

	if (!game->damage_effect.active)
		return ;
	intensity = calculate_flash_intensity(game);
	apply_damage_overlay(game, intensity);
}

void	player_take_damage(t_game *game, int damage)
{
	game->p1.health -= damage;
	if (game->p1.health < 0)
		game->p1.health = 0;
	trigger_damage_effect(game);
}
