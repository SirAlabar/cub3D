/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 20:33:08 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/10 21:03:02 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void draw_pixel(t_game *game, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;
    dst = game->addr[game->current_buffer] + 
        (y * game->line_length + x * (game->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void draw_texture_pixel(t_texture *tex, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
        return;
    dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
    *(unsigned int *)dst = color;
}

void draw_background(t_game *game)
{
    int x, y;
    int ceiling_color = game->map->ceiling_color;
    int floor_color = game->map->floor_color;
    
    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        x = 0;
        while (x < WINDOW_WIDTH)
        {
            if (y < WINDOW_HEIGHT / 2)
                draw_pixel(game, x, y, ceiling_color);
            else
                draw_pixel(game, x, y, floor_color);
            x++;
        }
        y++;
    }
}
