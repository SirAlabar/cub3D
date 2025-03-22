/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/22 20:08:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_minimap_cell(t_game *game, int screen_x, int screen_y, int color)
{
	int	start_x;
	int	start_y;
	int	i;
	int	j;

    start_x = MINIMAP_PADDING + (screen_x * MINIMAP_CELL_SIZE);
    start_y = WINDOW_HEIGHT - MINIMAP_PADDING - ((MINIMAP_VIEW_SIZE - screen_y)
            * MINIMAP_CELL_SIZE);
	i = -1;
	while (++i < MINIMAP_CELL_SIZE)
	{
		j = -1;
		while (++j < MINIMAP_CELL_SIZE)
			draw_pixel(game, start_x + j, start_y + i, color);
	}
}

static int	get_line_steps(t_vector start, t_vector end)
{
	double	diff_x;
	double	diff_y;

	diff_x = fabs(end.x - start.x);
	diff_y = fabs(end.y - start.y);
	if (diff_x > diff_y)
		return ((int)diff_x);
	return ((int)diff_y);
}

void	draw_direction_line(t_game *game, t_vector center, t_vector dir,
		int length)
{
	t_vector	end;
	t_vector	inc;
	int			steps;
	int			i;

	end.x = center.x + dir.x * length;
	end.y = center.y + dir.y * length;
	steps = get_line_steps(center, end);
	inc.x = (end.x - center.x) / (float)steps;
	inc.y = (end.y - center.y) / (float)steps;
	i = 0;
	while (i < steps)
	{
		draw_pixel(game, round(center.x), round(center.y), 0xFF0000);
		center.x += inc.x;
		center.y += inc.y;
		i++;
	}
}
