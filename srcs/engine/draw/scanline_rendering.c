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
    t_fixed32 cos_angle = fixed32_cos(game->p1.angle);
    t_fixed32 sin_angle = fixed32_sin(game->p1.angle);
    
    t_fixed32 dx = fixed32_sub(p.x, game->p1.pos.x);
    t_fixed32 dy = fixed32_sub(p.y, game->p1.pos.y);

    // Scale adjustment for transformation
    dx = fixed32_mul(dx, FIXED_POINT_SCALE);
    dy = fixed32_mul(dy, FIXED_POINT_SCALE);

    result.x = fixed32_div(
        fixed32_sub(fixed32_mul(dx, cos_angle), fixed32_mul(dy, sin_angle)),
        FIXED_POINT_SCALE
    );
    result.y = fixed32_div(
        fixed32_add(fixed32_mul(dx, sin_angle), fixed32_mul(dy, cos_angle)),
        FIXED_POINT_SCALE
    );

    return result;
}

void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
    t_fixed_vec32 v1, v2;
    int x1, x2, h1, h2;
    int start, end;
    int x, y;

    // Transform both endpoints
    v1 = transform_point(line->start, game);
    v2 = transform_point(line->end, game);

    // Debug log transformed points
    ft_printf("Transformed points: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(v1.x), fixed32_to_int(v1.y),
        fixed32_to_int(v2.x), fixed32_to_int(v2.y));

    // Early frustum culling
    if (v1.y < COLLISION_THRESHOLD && v2.y < COLLISION_THRESHOLD)
        return;

    // Project to screen space with FOV and fixed point scaling
    x1 = WINDOW_WIDTH/2 + fixed32_to_int(fixed32_div(
        fixed32_mul(v1.x, int_to_fixed32(WINDOW_WIDTH/2)),
        fixed32_div(v1.y, int_to_fixed32(FOV))
    ));
    x2 = WINDOW_WIDTH/2 + fixed32_to_int(fixed32_div(
        fixed32_mul(v2.x, int_to_fixed32(WINDOW_WIDTH/2)),
        fixed32_div(v2.y, int_to_fixed32(FOV))
    ));

    // Calculate wall heights with fixed point scaling
    h1 = fixed32_to_int(fixed32_mul(
        fixed32_div(int_to_fixed32(WINDOW_HEIGHT),
        fixed32_div(v1.y, FIXED_POINT_SCALE)),
        int_to_fixed32(2)
    ));
    h2 = fixed32_to_int(fixed32_mul(
        fixed32_div(int_to_fixed32(WINDOW_HEIGHT),
        fixed32_div(v2.y, FIXED_POINT_SCALE)),
        int_to_fixed32(2)
    ));

    // Debug log projected points
    ft_printf("Projected points: (%d,%d) -> (%d,%d) heights: %d,%d\n",
        x1, h1, x2, h2);

    // Order points from left to right
    if (x1 > x2)
    {
        int tmp;
        tmp = x1; x1 = x2; x2 = tmp;
        tmp = h1; h1 = h2; h2 = tmp;
    }

    // Clip to screen bounds
    start = x1;
    end = x2;
    if (start < 0) start = 0;
    if (end >= WINDOW_WIDTH) end = WINDOW_WIDTH - 1;

    // Draw wall slice by slice
    for (x = start; x <= end; x++)
    {
        // Interpolate height
        int h = h1;
        if (x2 != x1)
            h = h1 + ((h2 - h1) * (x - x1)) / (x2 - x1);

        int wall_top = (WINDOW_HEIGHT - h) / 2;
        int wall_bottom = (WINDOW_HEIGHT + h) / 2;

        // Clip vertical bounds
        if (wall_top < 0) wall_top = 0;
        if (wall_bottom >= WINDOW_HEIGHT) wall_bottom = WINDOW_HEIGHT - 1;

        // Draw wall slice
        for (y = wall_top; y <= wall_bottom; y++)
        {
            if (y >= buffer->y_top[x] && y <= buffer->y_bottom[x])
            {
                draw_pixel(game, x, y, 0x888888);
            }
        }

        buffer->y_top[x] = wall_bottom + 1;
    }
}