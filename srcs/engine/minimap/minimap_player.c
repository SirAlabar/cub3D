/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/16 20:25:15 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	draw_player_circle(t_game *game, t_vector center, int radius)
{
	int	i;
	int	j;

	i = -radius;
	while (i < radius)
	{
		j = -radius;
		while (j < radius)
		{
			if ((i * i + j * j) <= (radius * radius))
				draw_pixel(game, center.x + j, center.y + i, 0xFF0000);
			j++;
		}
		i++;
	}
}

void	draw_player_dot(t_game *game)
{
	t_vector	center;
	int			half_size;

	center.x = MINIMAP_PADDING + ((MINIMAP_VIEW_SIZE >> 1) * MINIMAP_CELL_SIZE)
		+ (MINIMAP_CELL_SIZE >> 1);
	center.y = WINDOW_HEIGHT - MINIMAP_PADDING - ((MINIMAP_VIEW_SIZE >> 1)
			* MINIMAP_CELL_SIZE) - (MINIMAP_CELL_SIZE >> 1);
	half_size = (PLAYER_DOT_SIZE >> 1);
	draw_player_circle(game, center, half_size);
	draw_direction_line(game, center, game->p1.dir, MINIMAP_CELL_SIZE);
}
