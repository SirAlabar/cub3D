/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/05 16:18:51 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	engine_prepare_frame(t_game *game)
{
	if (game->img && game->mlx)
	{
		mlx_destroy_image(game->mlx, game->img);
		game->img = NULL;
	}
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
}

void	engine_render_frame(t_game *game)
{
	engine_prepare_frame(game);
	draw_background(game);
	// TODO: Adicionar chamadas para raycast e draw_walls
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
