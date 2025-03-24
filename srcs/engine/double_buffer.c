/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:48:48 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:48:50 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_double_buffer(t_game *game)
{
	if (!game || !game->mlx)
	{
		ft_putendl_fd("Error: Cannot initialize double buffer", 2);
		return ;
	}
	game->img[0] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	game->img[1] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img[0] || !game->img[1])
	{
		ft_putendl_fd("Error: Failed to create rendering buffers", 2);
		cleanup_game(game);
		exit(1);
	}
	game->addr[0] = mlx_get_data_addr(game->img[0], &game->bits_per_pixel,
			&game->line_length, &game->endian);
	game->addr[1] = mlx_get_data_addr(game->img[1], &game->bits_per_pixel,
			&game->line_length, &game->endian);
	game->current_buffer = 0;
}

void	swap_buffers(t_game *game)
{
	if (!game || !game->mlx || !game->win || !game->img[game->current_buffer])
		return ;
	mlx_put_image_to_window(game->mlx, game->win,
		game->img[game->current_buffer], 0, 0);
	game->current_buffer = !game->current_buffer;
}
