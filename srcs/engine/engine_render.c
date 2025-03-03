/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:51:43 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	engine_render_frame(t_game *game)
{
	// // int			x;
	// t_ray		rays[WINDOW_WIDTH];
	// t_scanline	scanline_buffer;

	if (!game || !game->addr[game->current_buffer])
		return (0);
	update_doors(game);
	draw_background(game);
	// init_scanline_buffer(&scanline_buffer);
	// cast_rays(game, rays);
	// x = -1;
	// while (++x < WINDOW_WIDTH)
	// {
	// 	draw_wall_scanline(game, &rays[x], x, &scanline_buffer);
	// }
	draw_enemies(game);
	handle_movement(game);
	update_weapon_animation(game);
	draw_weapon(game);
	draw_minimap(game);
	draw_health_bar(game);
	swap_buffers(game);
	return (0);
}
