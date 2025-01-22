/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_bar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:32:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/22 17:49:25 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	draw_health_bar_background(t_game *game)
{
	int	start_x;
	int	start_y;
	int	width;
	int	height;
	int	i;
	int	j;

	width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE;
	height = 20;
	start_x = MINIMAP_PADDING;
	start_y = WINDOW_HEIGHT - MINIMAP_PADDING - height;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
			draw_pixel(game, start_x + j, start_y + i + 30, 0xFF3000);
	}
}

static void	draw_health_bar_fill(t_game *game)
{
	int		start_x;
	int		start_y;
	int		width;
	int		height;
	int		fill_width;
	int		i;
	int		j;
	int		color;

	width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE;
	height = 20;
	start_x = MINIMAP_PADDING;
	start_y = WINDOW_HEIGHT - MINIMAP_PADDING - height;
	fill_width = (width * game->p1.health) / 100;
	color = 0xFF0000;
	if (game->p1.health > 50)
		color = 0xFF0000;
	else if (game->p1.health > 25)
		color = 0x00FF00;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < fill_width)
			draw_pixel(game, start_x + j, start_y + i + 30, color);
	}
}

void	draw_health_bar(t_game *game)
{
	draw_health_bar_background(game);
	draw_health_bar_fill(game);
}
