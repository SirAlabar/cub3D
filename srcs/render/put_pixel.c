/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 20:34:44 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/11/30 21:15:26 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	put_pixel(t_texture *tex, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
	{
		ft_printf("Error: Attempt to draw outside the image bounds (%d, %d)\n",
			x, y);
		return ;
	}
	dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	*(unsigned int *)dst = color;
}

unsigned int	get_color_in_pixel(t_texture *tex, int x, int y)
{
	unsigned int	color;

	color = *(unsigned int *)(tex->addr + (y * tex->line_len + x * (tex->bpp
					/ 8)));
	return (color);
}
