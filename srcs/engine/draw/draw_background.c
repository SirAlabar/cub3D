/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_background.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/05 18:28:06 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	draw_background(t_game *game)
{
	int	x;
	int	y;

	if (!game || !game->img)
	{
		ft_printf("Error: Game or image pointer is NULL\n");
		return (0);
	}
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		x = -1;
		while (++x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				draw_pixel(game, x, y, game->map.ceiling_color);
			else
				draw_pixel(game, x, y, game->map.floor_color);
		}
	}
	return (1);
}
