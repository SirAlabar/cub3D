/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:20:17 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 16:06:32 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_text_box(t_game *game, int x, int y, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 40)
	{
		j = -1;
		while (++j < 20)
			draw_pixel(game, x + i, y + j, 0x000000);
	}
	i = -1;
	while (++i < 38)
	{
		j = -1;
		while (++j < 18)
			draw_pixel(game, x + i + 1, y + j + 1, color);
	}
}

void	draw_hud(t_game *game)
{
	char	*fps;

	fps = ft_itoa(game->fps);
	draw_text_box(game, 10, 10, 0x333333);
	mlx_string_put(game->mlx, game->win, 15, 25, 0xFFFFFF, fps);
	free(fps);
}
