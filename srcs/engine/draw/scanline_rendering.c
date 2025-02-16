/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/13 19:20:04 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <cub3d.h>
// Base world to screen scale: 1 world unit = 64 screen pixels at 1 unit distance
#define WORLD_TO_SCREEN_SCALE (64 * FIXED_POINT_SCALE)

void init_scanline_buffer(t_scanline *buffer)
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



static t_fixed_vec32 transform_point(t_fixed_vec32 p, t_game *game)
{
    t_fixed_vec32 result;
    
    // Translate to player space
    t_fixed32 dx = fixed32_sub(p.x, game->p1.pos.x);
    t_fixed32 dy = fixed32_sub(p.y, game->p1.pos.y);
    
    // Use player's forward direction for view transform
    t_fixed32 cos_val = fixed32_cos(game->p1.angle);
    t_fixed32 sin_val = fixed32_sin(game->p1.angle);
    
    // Transform to view space (rotação invertida porque estamos transformando para view space)
    result.x = fixed32_add(
        fixed32_mul(dx, cos_val),
        fixed32_mul(dy, sin_val)
    );
    
    result.y = fixed32_sub(
        fixed32_mul(dy, cos_val),
        fixed32_mul(dx, sin_val)
    );

    return result;
}

void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
    t_fixed_vec32 v1, v2;
    
    // Transform vertices to view space
    v1 = transform_point(line->start, game);
    v2 = transform_point(line->end, game);

    // Early frustum culling
    if (v1.y < (FIXED_POINT_SCALE >> 2) && v2.y < (FIXED_POINT_SCALE >> 2))
        return;

    // Project to screen space with fixed scaling
    int x1 = WINDOW_WIDTH/2 + fixed32_to_int(
        fixed32_div(fixed32_mul(v1.x, int_to_fixed32(WINDOW_WIDTH)),
        v1.y << 1)
    );
    
    int x2 = WINDOW_WIDTH/2 + fixed32_to_int(
        fixed32_div(fixed32_mul(v2.x, int_to_fixed32(WINDOW_WIDTH)),
        v2.y << 1)
    );

    // Calculate heights in screen space
    int h1 = fixed32_to_int(fixed32_div(int_to_fixed32(WINDOW_HEIGHT << 1), v1.y));
    int h2 = fixed32_to_int(fixed32_div(int_to_fixed32(WINDOW_HEIGHT << 1), v2.y));

    // Clip coordinates
    if (x1 < 0) x1 = 0;
    if (x2 >= WINDOW_WIDTH) x2 = WINDOW_WIDTH - 1;
    if (x1 >= x2) return;

    // Draw vertical slices
    for (int x = x1; x <= x2; x++)
    {
        // Interpolate height
        int t = ((x - x1) << FIXED_POINT_BITS) / (x2 - x1);
        int h = h1 + (((h2 - h1) * t) >> FIXED_POINT_BITS);
        
        // Calculate top and bottom screen coordinates
        int wall_center = WINDOW_HEIGHT/2;
        int top = wall_center - (h >> 1);
        int bottom = wall_center + (h >> 1);

        // Clip to current scanline buffer
        if (top < buffer->y_top[x]) top = buffer->y_top[x];
        if (bottom > buffer->y_bottom[x]) bottom = buffer->y_bottom[x];

        // Draw if visible
        if (top <= bottom)
        {
            // Interpolate depth for shading
            t_fixed32 depth = fixed32_add(v1.y,
                fixed32_mul(fixed32_sub(v2.y, v1.y),
                int_to_fixed32(x - x1) / (x2 - x1)));

            // Calculate shade based on depth
            int shade = fixed32_to_int(fixed32_div(FIXED_POINT_SCALE << 6, depth));
            if (shade > 255) shade = 255;
            if (shade < 32) shade = 32;

            // Draw vertical slice
            for (int y = top; y <= bottom; y++)
            {
                int color = (shade << 16) | (shade << 8) | shade;
                draw_pixel(game, x, y, color);
            }

            // Update scanline buffer
            buffer->y_top[x] = bottom + 1;
        }
    }
}