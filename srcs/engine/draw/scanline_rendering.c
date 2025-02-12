/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 18:20:04 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void    init_scanline_buffer(t_scanline *buffer)
{
    int x;

    x = -1;
    while (++x < WINDOW_WIDTH)
    {
        buffer->y_ceil[x] = 0;
        buffer->y_floor[x] = WINDOW_HEIGHT - 1;
        buffer->y_top[x] = 0;
        buffer->y_bottom[x] = WINDOW_HEIGHT - 1;
    }
}

static void    put_wall_pixel(t_wall *wall, t_vector_i pos)
{
    int     pixel_pos;
    char    *current_buffer;

    current_buffer = wall->game->addr[wall->game->current_buffer];
    wall->tex.y = (int)wall->tex_pos & (wall->texture->height - 1);
    wall->color = get_texture_pixel(wall->texture, wall->tex.x, wall->tex.y);
    wall->color = apply_shade(wall->color, fixed32_to_float(
        fixed32_div(FIXED_POINT_SCALE, 
            fixed32_add(FIXED_POINT_SCALE,
                fixed32_mul(wall->ray->hit_info.distance,
                    int_to_fixed32(5) / 100)))));
    pixel_pos = (pos.y * wall->game->line_length) + 
        (pos.x * (wall->game->bits_per_pixel / 8));
    *(unsigned int *)(current_buffer + pixel_pos) = wall->color;
    wall->tex_pos += wall->step;
}

void    set_wall_tex_coords(t_wall *wall)
{
    t_fixed_vec32 hit_point;

    hit_point = wall->ray->hit_info.point;
    wall->pos.x = fixed32_sub(hit_point.x, 
        fixed32_mul(int_to_fixed32(fixed32_to_int(hit_point.x)), 
            FIXED_POINT_SCALE));
    wall->tex.x = fixed32_to_int(fixed32_mul(wall->pos.x, 
        int_to_fixed32(wall->texture->width)));
    if (wall->ray->hit_info.side == 0 && wall->ray->dir.x < 0)
        wall->tex.x = wall->texture->width - wall->tex.x - 1;
    if (wall->ray->hit_info.side == 1 && wall->ray->dir.y > 0)
        wall->tex.x = wall->texture->width - wall->tex.x - 1;
}

void    init_wall_drawing(t_wall *wall)
{
    wall->height = fixed32_div(int_to_fixed32(WINDOW_HEIGHT), 
        wall->ray->hit_info.distance);
    wall->start = WINDOW_HEIGHT / 2 - fixed32_to_int(wall->height) / 2;
    wall->end = wall->start + fixed32_to_int(wall->height);
    if (wall->start < wall->buffer->y_top[wall->x])
        wall->start = wall->buffer->y_top[wall->x];
    if (wall->end > wall->buffer->y_bottom[wall->x])
        wall->end = wall->buffer->y_bottom[wall->x];
    wall->texture = get_wall_texture(wall->ray, wall->game);
    set_wall_tex_coords(wall);
    wall->step = (double)wall->texture->height / fixed32_to_int(wall->height);
    wall->tex_pos = (wall->start - WINDOW_HEIGHT / 2 + 
        fixed32_to_int(wall->height) / 2) * wall->step;
}

void    draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
{
    t_wall      wall;
    t_vector_i  pos;

    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
    init_wall_drawing(&wall);
    pos.x = x;
    pos.y = wall.start - 1;
    while (++pos.y <= wall.end)
        put_wall_pixel(&wall, pos);
    buffer->y_top[x] = wall.end + 1;
    buffer->y_bottom[x] = wall.start - 1;
}