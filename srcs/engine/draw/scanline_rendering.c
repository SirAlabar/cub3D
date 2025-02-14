/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/14 19:34:56 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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
    t_fixed32 cos_val, sin_val;
    
    // Translate point relative to player
    t_fixed32 dx = fixed32_sub(p.x, game->p1.pos.x);
    t_fixed32 dy = fixed32_sub(p.y, game->p1.pos.y);
    
    // Get sine and cosine from tables
    cos_val = get_cos_8192(game->fixed_tables, game->p1.angle);
    sin_val = get_sin_8192(game->fixed_tables, game->p1.angle);
    
    // Rotate point around player
    result.x = fixed32_sub(
        fixed32_mul(dx, cos_val),
        fixed32_mul(dy, sin_val)
    );
    
    result.y = fixed32_add(
        fixed32_mul(dx, sin_val),
        fixed32_mul(dy, cos_val)
    );

    return result;
}

void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
    t_fixed_vec32 v1, v2;
    
    // Transform vertices to view space
    v1 = transform_point(line->start, game);
    v2 = transform_point(line->end, game);

    // Skip if behind player
    if (v1.y <= 0 && v2.y <= 0)
        return;

    // Project to screen space
    int x1 = WINDOW_WIDTH/2;
    int x2 = WINDOW_WIDTH/2;
    int h1 = 0, h2 = 0;
    
    if (v1.y > 0) {
        x1 += fixed32_to_int(fixed32_mul(v1.x, FIXED_POINT_SCALE) / v1.y);
        h1 = fixed32_to_int(fixed32_mul(int_to_fixed32(WINDOW_HEIGHT), 
             FIXED_POINT_SCALE) / v1.y);
    }
    
    if (v2.y > 0) {
        x2 += fixed32_to_int(fixed32_mul(v2.x, FIXED_POINT_SCALE) / v2.y);
        h2 = fixed32_to_int(fixed32_mul(int_to_fixed32(WINDOW_HEIGHT), 
             FIXED_POINT_SCALE) / v2.y);
    }

    // Order points left to right
    if (x1 > x2) {
        int tmp;
        tmp = x1; x1 = x2; x2 = tmp;
        tmp = h1; h1 = h2; h2 = tmp;
        t_fixed_vec32 temp = v1;
        v1 = v2;
        v2 = temp;
    }

    // Clip to screen bounds
    int start = (x1 < 0) ? 0 : x1;
    int end = (x2 >= WINDOW_WIDTH) ? WINDOW_WIDTH - 1 : x2;

    // Draw wall slices
    for (int x = start; x <= end; x++) {
        float t = (x2 == x1) ? 0.0f : (float)(x - x1) / (float)(x2 - x1);
        int h = h1 + (int)(t * (float)(h2 - h1));
        
        int wall_top = (WINDOW_HEIGHT - h) / 2;
        int wall_bottom = (WINDOW_HEIGHT + h) / 2;
        
        wall_top = (wall_top < 0) ? 0 : wall_top;
        wall_bottom = (wall_bottom >= WINDOW_HEIGHT) ? WINDOW_HEIGHT - 1 : wall_bottom;

        for (int y = wall_top; y <= wall_bottom; y++) {
            if (y >= buffer->y_top[x] && y <= buffer->y_bottom[x]) {
                // Basic distance shading
                t_fixed32 dist = fixed32_add(v1.y, 
                    fixed32_mul(fixed32_sub(v2.y, v1.y), int_to_fixed32(t * FIXED_POINT_SCALE)));
                
                int shade = 0xFF - fixed32_to_int(fixed32_mul(dist, int_to_fixed32(1)));
                shade = (shade < 0x40) ? 0x40 : (shade > 0xFF) ? 0xFF : shade;
                
                int color = (shade << 16) | (shade << 8) | shade;
                draw_pixel(game, x, y, color);
            }
        }
        buffer->y_top[x] = wall_bottom + 1;
    }
}