/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_background.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/07 12:26:30 by marsoare         ###   ########.fr       */
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

void	draw_wall(t_game *game, t_ray *ray, int x)
{
	int	y;
	int	color;

	if (ray->side == 0)
	{
		color = 0x606060;
	}
	else
	{
		color = 0x808080;
	}
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		draw_pixel(game, x, y, color);
		y++;
	}
}
