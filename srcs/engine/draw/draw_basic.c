/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:39 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 23:04:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_pixel(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (!game || !game->addr)
		return ;
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
    if (!tex || !tex->addr || !tex->img ||
        x < 0 || x >= tex->width ||
        y < 0 || y >= tex->height)
    {
        return 0xFF000000;
    }	
	return (*(unsigned int *)(tex->addr + (y * tex->line_len + x * (tex->bpp
				/ 8))));
}

unsigned int	apply_shade(unsigned int color, double shade)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = ((color >> 16) & 0xFF) * shade;
	g = ((color >> 8) & 0xFF) * shade;
	b = (color & 0xFF) * shade;
	return ((0xFF << 24) | (r << 16) | (g << 8) | b);
}
