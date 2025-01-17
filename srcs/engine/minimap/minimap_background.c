/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_background.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/16 20:37:21 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static unsigned int	darken_color(unsigned int original, float factor)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = ((original >> 16) & 0xFF) * factor;
	g = ((original >> 8) & 0xFF) * factor;
	b = (original & 0xFF) * factor;
	return ((0xFF << 24) | (r << 16) | (g << 8) | b);
}

static unsigned int	get_pixel_color(t_game *game, int x, int y)
{
	char	*pixel;

	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return (0);
	pixel = game->addr + (y * game->line_length
			+ x * (game->bits_per_pixel / 8));
	return (*(unsigned int *)pixel);
}

static void	process_pixel_color(t_game *game, t_vector pos, int base_y)
{
	unsigned int	original;
	unsigned int	darkened;

	original = get_pixel_color(game, pos.x, base_y + pos.y);
	darkened = darken_color(original, 0.5);
	draw_pixel(game, pos.x, base_y + pos.y, darkened);
}

void	draw_minimap_background(t_game *game)
{
	int			width;
	int			height;
	int			base_y;
	t_vector	pos;

	width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING * 2);
	height = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING * 2);
	base_y = WINDOW_HEIGHT - height;
	pos.y = -1;
	while (++pos.y < height)
	{
		pos.x = -1;
		while (++pos.x < width)
		{
			if (pos.y < MINIMAP_PADDING || pos.y >= height - MINIMAP_PADDING
				|| pos.x < MINIMAP_PADDING || pos.x >= width - MINIMAP_PADDING)
				continue ;
			process_pixel_color(game, pos, base_y);
		}
	}
}
