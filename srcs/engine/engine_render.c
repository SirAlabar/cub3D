/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/09 11:38:31 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	render_player_weapons(t_game *game, t_ray *rays,
	t_scanline *scanline_buffer)
{
	if (game->portal_system && game->portal_system->portal_active)
	{
		render_portals(game, rays, scanline_buffer);
	}
	if (game->active_weapon == 0)
	{
		update_weapon_animation(game);
		draw_weapon(game);
	}
	else if (game->portal_system)
	{
		update_portal_gun_animation(game);
		draw_portal_gun(game);
	}
}

int	render_game_world(t_game *game, t_ray *rays, t_scanline *scanline_buffer)
{
	int	x;

	draw_background(game);
	init_scanline_buffer(scanline_buffer);
	cast_rays(game, rays);
	x = -1;
	while (++x < WINDOW_WIDTH)
		draw_wall_scanline(game, &rays[x], x, scanline_buffer);
	update_enemies(game);
	update_enemy_animations(game);
	update_damage_effect(game);
	draw_enemies(game);
	return (0);
}

int	engine_render_frame(t_game *game)
{
	t_ray		rays[WINDOW_WIDTH];
	t_scanline	scanline_buffer;

	if (!game || !game->addr[game->current_buffer])
		return (0);
	update_doors(game);
	update_portals(game);
	process_enemy_attacks(game);
	render_game_world(game, rays, &scanline_buffer);
	handle_movement(game);
	render_player_weapons(game, rays, &scanline_buffer);
	draw_minimap(game);
	draw_health_bar(game);
	draw_damage_effect(game);
	swap_buffers(game);
	return (0);
}
