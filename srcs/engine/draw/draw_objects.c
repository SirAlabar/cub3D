/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/15 20:27:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void draw_object_pixel(t_game *game, t_vector_i pos, t_texture *sprite,
        double transform_y)
{
    t_texture   *tex;
    int         d;
    int         tex_y;
    int         color;

    tex = sprite->frames[sprite->current_frame];
    if (!tex)
        return ;
    d = (pos.y * TEXTURE_SCALE - WINDOW_HEIGHT * TEXTURE_HALF_SCALE 
         + sprite->tex.y * TEXTURE_HALF_SCALE);
    tex_y = ((d * tex->height) / sprite->tex.y) / TEXTURE_SCALE;
    color = get_texture_pixel(tex, sprite->tex.x, tex_y);
    if (color != 0xFFC0CB)
    {
        color = apply_shade(color, 1.0 / (1.0 + transform_y * 0.1));
        draw_pixel(game, pos.x, pos.y, color);
    }
}

void    draw_object(t_game *game, t_texture *sprite, double transform_y)
{
    t_vector_i  pos;
    int         draw_start;
    int         draw_end;

    if (!sprite->frames[sprite->current_frame])
        return ;
    draw_start = -sprite->tex.y / 2 + WINDOW_HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = sprite->tex.y / 2 + WINDOW_HEIGHT / 2;
    if (draw_end >= WINDOW_HEIGHT)
        draw_end = WINDOW_HEIGHT - 1;
    pos.y = draw_start - 1;
    while (++pos.y < draw_end)
    {
        pos.x = sprite->tex.x;
        draw_object_pixel(game, pos, sprite, transform_y);
    }
}