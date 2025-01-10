/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/10 21:09:09 by marvin           ###   ########.fr       */
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
	int i;
	t_ray rays[WINDOW_WIDTH];

	engine_prepare_frame(game);
	draw_background(game);
	cast_rays(game, rays);
	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		draw_wall(game, &rays[i], i);
	}			
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
