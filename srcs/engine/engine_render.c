/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/12 14:04:53 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	engine_prepare_frame(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (game->img)
	{
		mlx_destroy_image(game->mlx, game->img);
		game->img = NULL;
	}
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return ;
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
}

int	engine_render_frame(t_game *game)
{
	int			x;
	t_ray		rays[WINDOW_WIDTH];
	t_scanline	scanline_buffer;

	update_fps(game);
	engine_prepare_frame(game);
	draw_background(game);
	init_scanline_buffer(&scanline_buffer);
	cast_rays(game, rays);
	x = -1;
	while (++x < WINDOW_WIDTH)
	{
		draw_wall_scanline(game, &rays[x], x, &scanline_buffer);
		// draw_wall(game, &rays[x], x);
	}
	handle_movement(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	draw_hud(game);
	return (0);
}
