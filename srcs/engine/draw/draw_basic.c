/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/05 16:34:37 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return ;
	dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	draw_texture_pixel(t_texture *tex, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
	{
		ft_printf("Error: Attempt to draw outside texture bounds (%d, %d)\n", x,
			y);
		return ;
	}
	dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	*(unsigned int *)dst = color;
}

unsigned int	get_texture_pixel(t_texture *tex, int x, int y)
{
	return (*(unsigned int *)(tex->addr + (y * tex->line_len + x * (tex->bpp
				/ 8))));
}
