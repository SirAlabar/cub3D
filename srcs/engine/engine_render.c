/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/13 18:06:00 by hluiz-ma         ###   ########.fr       */
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

static void	render_node_geometry(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
	int	i;

	i = -1;
	while (++i < node->num_lines)
		draw_line_segment(game, node->lines[i], buffer);
}

void	render_bsp_node(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
	t_bsp_side	side;

	if (!node)
		return ;
	if (!node->partition)
	{
		render_node_geometry(game, node, buffer);
		return ;
	}
	side = bsp_classify_point(game->p1.pos, node->partition);
	if (side == BSP_FRONT)
	{
		render_bsp_node(game, node->back, buffer);
		render_node_geometry(game, node, buffer);
		render_bsp_node(game, node->front, buffer);
	}
	else
	{
		render_bsp_node(game, node->front, buffer);
		render_node_geometry(game, node, buffer);
		render_bsp_node(game, node->back, buffer);
	}
}

int	render_frame(t_game *game)
{
	t_scanline	buffer;

	init_scanline_buffer(&buffer);
	render_bsp_node(game, game->bsp_tree->root, &buffer);
	draw_weapon(game);
	swap_buffers(game);
	return (0);
}