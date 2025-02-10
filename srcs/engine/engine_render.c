/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/10 20:44:50 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_double_buffer(t_game *game)
{
	game->buffer[0] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	game->buffer[1] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->buffer[0] || !game->buffer[1])
		cleanup_game(game);
	game->addr[0] = mlx_get_data_addr(game->buffer[0], &game->bits_per_pixel,
			&game->line_length, &game->endian);
	game->addr[1] = mlx_get_data_addr(game->buffer[1], &game->bits_per_pixel,
			&game->line_length, &game->endian);
	game->current_buffer = 0;
}

void	swap_buffers(t_game *game)
{
	mlx_put_image_to_window(game->mlx, game->win,
		game->buffer[game->current_buffer], 0, 0);
	game->current_buffer = !game->current_buffer;
}

void	cleanup_double_buffer(t_game *game)
{
	if (game->buffer[0])
		mlx_destroy_image(game->mlx, game->buffer[0]);
	if (game->buffer[1])
		mlx_destroy_image(game->mlx, game->buffer[1]);
}

int	render_frame(t_game *game)
{
	t_ray		*rays;
	t_scanline	buffer;

	mlx_sync(MLX_SYNC_IMAGE_WRITABLE, game->buffer[game->current_buffer]);
	rays = ft_calloc(1, (sizeof(t_ray) * WINDOW_WIDTH));
	if (!rays)
		return (0);
	init_scanline_buffer(&buffer);
	cast_rays(game, rays);
	free(rays);
	return (0);
}
