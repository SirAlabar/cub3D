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

static void	process_minimap_pixel(t_game *g, t_vector pos, int base_y)
{
	char			*pixel;
	unsigned int	color;
	int				y_pos;
	int				offset;

	y_pos = base_y + (int)pos.y;
	if (pos.x < 0 || pos.x >= WINDOW_WIDTH || y_pos < 0
		|| y_pos >= WINDOW_HEIGHT)
		return ;
	offset = (y_pos * g->line_length) + ((int)pos.x * (g->bits_per_pixel >> 3));
	pixel = g->addr[g->current_buffer] + offset;
	color = *(unsigned int *)pixel;
	color = apply_shade(color, 0.5);
	draw_pixel(g, (int)pos.x, y_pos, color);
}

void	draw_minimap_background(t_game *game)
{
	int			width;
	int			height;
	int			base_y;
	t_vector	pos;

	width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING << 1);
	height = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING << 1);
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
			process_minimap_pixel(game, pos, base_y);
		}
	}
}
