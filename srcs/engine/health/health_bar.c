/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_bar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:37:30 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/25 10:46:20 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_bar	init_bar(void)
{
	t_bar	bar;

	bar.width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE;
	bar.height = 20;
	bar.start_x = MINIMAP_PADDING;
	bar.start_y = WINDOW_HEIGHT - MINIMAP_PADDING - bar.height;
	return (bar);
}

static void	draw_health_bar_background(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;

	bar = init_bar();
	bar.color = 0xFF3000;
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < bar.width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i + 30, bar.color);
	}
}

static void	draw_health_bar_fill(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;
	int		fill_width;

	bar = init_bar();
	fill_width = (bar.width * game->p1.health) / 100;
	if (game->p1.health > 50)
		bar.color = 0xFF0000;
	else
		bar.color = 0x00FF00;
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < fill_width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i + 30, bar.color);
	}
}

void	draw_health_bar(t_game *game)
{
	draw_health_bar_background(game);
	draw_health_bar_fill(game);
}
