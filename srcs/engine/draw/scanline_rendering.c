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
// No topo do arquivo ou em um header
#define PROJ_SCALE 39321600  
#define VIEW_DIST 16777216   
#define NEAR_PLANE 65536     
#define FAR_PLANE 65536000


// Inicialização corrigida
t_render_debug g_debug = {
    .enabled = false,
    .current_wall = 0,
    .wall_debug = {
        .v1_world = {0, 0},
        .v1_view = {0, 0},
        .v2_world = {0, 0},
        .v2_view = {0, 0},
        .x1_screen = 0,
        .x2_screen = 0,
        .h1_screen = 0,
        .h2_screen = 0
    }
};
void debug_render_wall_segment(t_game *game, t_bsp_line *line) {
    (void)game;
    if (!g_debug.enabled) return;
   
    g_debug.current_wall++;
    g_debug.wall_debug.v1_world = line->start;
    g_debug.wall_debug.v2_world = line->end;
   
    printf("\n=== Wall %d ===\n", g_debug.current_wall);
    printf("World coords: (fixed: %d,%d) (int: %d,%d) to (fixed: %d,%d) (int: %d,%d)\n",
           line->start.x, line->start.y,
           fixed32_to_int(line->start.x), fixed32_to_int(line->start.y),
           line->end.x, line->end.y,
           fixed32_to_int(line->end.x), fixed32_to_int(line->end.y));
}

void debug_after_wall_transform(t_fixed_vec32 v1, t_fixed_vec32 v2) {
    if (!g_debug.enabled) return;
   
    g_debug.wall_debug.v1_view = v1;
    g_debug.wall_debug.v2_view = v2;
   
    printf("View space: (fixed: %d,%d) (int: %d,%d) to (fixed: %d,%d) (int: %d,%d)\n",
           v1.x, v1.y,
           fixed32_to_int(v1.x), fixed32_to_int(v1.y),
           v2.x, v2.y,
           fixed32_to_int(v2.x), fixed32_to_int(v2.y));
}

void debug_transform_details(t_fixed_vec32 p, t_game *game,
    t_fixed32 dx, t_fixed32 dy,
    t_fixed32 cos_val, t_fixed32 sin_val,
    t_fixed_vec32 result) {
    if (!g_debug.enabled) return;
    
    printf("\nTransform Details:\n");
    printf("Player pos (fixed): (%d, %d) (int): (%d, %d)\n",
           game->p1.pos.x, game->p1.pos.y,
           fixed32_to_int(game->p1.pos.x), fixed32_to_int(game->p1.pos.y));
    // printf("Player angle (raw): %d (deg): %f\n",
    //        game->p1.angle, (float)game->p1.angle * 360.0f / ANG360);
    printf("Input point (fixed): (%d, %d) (int): (%d, %d)\n",
           p.x, p.y,
           fixed32_to_int(p.x), fixed32_to_int(p.y));
    printf("Delta (fixed): (%d, %d) (int): (%d, %d)\n",
           dx, dy,
           fixed32_to_int(dx), fixed32_to_int(dy));
    printf("Cos/Sin (fixed): (%d, %d) (int): (%d, %d)\n",
           cos_val, sin_val,
           fixed32_to_int(cos_val), fixed32_to_int(sin_val));

    // Verificação de sanidade aqui
    if (abs(fixed32_to_int(cos_val)) > 1 || abs(fixed32_to_int(sin_val)) > 1) {
        printf("WARNING: Trig values out of range!\n");
        printf("Raw cos: %d sin: %d\n", cos_val, sin_val);
        printf("Angle index: %d\n", (game->p1.angle & ANGLEMASK) >> ANGLETOFINESHIFT);
    }

    printf("Result (fixed): (%d, %d) (int): (%d, %d)\n",
           result.x, result.y,
           fixed32_to_int(result.x), fixed32_to_int(result.y));
}

void debug_projection_details(t_fixed_vec32 view_pos, int screen_x, int height) {
    if (!g_debug.enabled) return;
    
    printf("\nProjection Details:\n");
    printf("View pos (fixed): (%d, %d) (int): (%d, %d)\n",
           view_pos.x, view_pos.y,
           fixed32_to_int(view_pos.x), fixed32_to_int(view_pos.y));
	// printf("FOV factor (fixed): %d (int): %d\n", 
	// 		DOOM_UNITS_PER_TILE, fixed32_to_int(DOOM_UNITS_PER_TILE));
    printf("Screen center offset: %d\n", WINDOW_WIDTH/2);
    printf("Projected x: %d\n", screen_x);
    printf("Wall height: %d\n", height);
}

void debug_scanline_details(int x, int top, int bottom, int h, t_fixed32 depth) {
    if (!g_debug.enabled) return;
    static int last_x = -1;
    
    if (x % 50 == 0 && x != last_x) {
        printf("\nScanline at x=%d:\n", x);
        printf("Height: %d\n", h);
        printf("Y range: %d to %d\n", top, bottom);
        printf("Depth (fixed): %d (int): %d\n",
               depth, fixed32_to_int(depth));
        last_x = x;
    }
}

// void debug_after_wall_projection(int x1, int x2, int h1, int h2) {
//     if (!g_debug.enabled) return;
   
//     g_debug.wall_debug.x1_screen = x1;
//     g_debug.wall_debug.x2_screen = x2;
//     g_debug.wall_debug.h1_screen = h1;
//     g_debug.wall_debug.h2_screen = h2;
   
//     printf("Screen space coordinates:\n");
//     printf("X range: %d to %d\n", x1, x2);
//     printf("Heights: start=%d end=%d\n", h1, h2);
// }

/*
static void debug_scale_info(t_fixed_vec32 vertex, const char* point_name) {
    if (!g_debug.enabled) return;

    // Valor original em fixed point
    printf("\n=== Scale Debug for %s ===\n", point_name);
    printf("Fixed point values:\n");
    printf("  x: %d, y: %d\n", vertex.x, vertex.y);
    
    // Convertendo back para inteiros
    int x_int = fixed32_to_int(vertex.x);
    int y_int = fixed32_to_int(vertex.y);
    printf("Integer values (fixed/65536):\n");
    printf("  x: %d, y: %d\n", x_int, y_int);
    
    // Escala DOOM (dividindo por 64)
    printf("DOOM tile scale (int/64):\n");
    printf("  x: %d tile(s), y: %d tile(s)\n", x_int/64, y_int/64);
    
    // Verificando se os valores estão alinhados com a grade
    if (x_int % 64 != 0 || y_int % 64 != 0) {
        printf("WARNING: Coordinates not aligned to DOOM's 64-unit grid!\n");
    }
}

// Função para debug de transformação de coordenadas
static void debug_transform_scale(t_fixed_vec32 world_pos, t_fixed_vec32 view_pos, 
                         const char* vertex_name) {
    if (!g_debug.enabled) return;

    printf("\n=== Transform Debug for %s ===\n", vertex_name);
    printf("World Space:\n");
    printf("  fixed: (%d, %d)\n", world_pos.x, world_pos.y);
    printf("  int: (%d, %d)\n", fixed32_to_int(world_pos.x), 
                                fixed32_to_int(world_pos.y));
    printf("View Space:\n");
    printf("  fixed: (%d, %d)\n", view_pos.x, view_pos.y);
    printf("  int: (%d, %d)\n", fixed32_to_int(view_pos.x), 
                                fixed32_to_int(view_pos.y));
    
    // Verificar distância do jogador
    t_fixed32 distance = fixed32_sqrt(
        fixed32_add(
            fixed32_mul(view_pos.x, view_pos.x),
            fixed32_mul(view_pos.y, view_pos.y)
        )
    );
    
    printf("Distance to player: fixed=%d (int=%d units, %d tiles)\n",
           distance, 
           fixed32_to_int(distance),
           fixed32_to_int(distance)/64);
}

// Função para debug do jogador
static void debug_player_scale(t_game *game) {
    if (!g_debug.enabled) return;

    printf("\n=== Player Scale Debug ===\n");
    printf("Position:\n");
    printf("  fixed: (%d, %d)\n", game->p1.pos.x, game->p1.pos.y);
    printf("  int: (%d, %d)\n", fixed32_to_int(game->p1.pos.x), 
                                fixed32_to_int(game->p1.pos.y));
    printf("  tile: (%d, %d)\n", fixed32_to_int(game->p1.pos.x)/64, 
                                 fixed32_to_int(game->p1.pos.y)/64);
    printf("Angle:\n");
    printf("  raw: %d\n", game->p1.angle);
    printf("  degrees: %f\n", (float)game->p1.angle * 360.0f / ANG360);
}
*/






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
/*
static t_fixed32 clamp(t_fixed32 val, t_fixed32 min, t_fixed32 max) {
    if (val <= min) return min;
    if (val >= max) return max;
    return val;
}*/

t_fixed_vec32 transform_point(t_fixed_vec32 p, t_game *game)
{
    t_fixed_vec32 result;
    t_fixed32 dx, dy;
    t_fixed32 cos_val, sin_val;

    printf("\n=== Transform Point Debug ===\n");
    
    dx = fixed32_sub(p.x, game->p1.pos.x);
    dy = fixed32_sub(p.y, game->p1.pos.y);
    
    printf("Point: (%d,%d)\n", fixed32_to_int(p.x), fixed32_to_int(p.y));
    printf("Player: (%d,%d)\n", fixed32_to_int(game->p1.pos.x), fixed32_to_int(game->p1.pos.y));
    printf("Delta: (%d,%d)\n", fixed32_to_int(dx), fixed32_to_int(dy));
    
    cos_val = get_cos_8192(game->fixed_tables, game->p1.angle);
    sin_val = get_sin_8192(game->fixed_tables, game->p1.angle);
    
    result.x = fixed32_sub(
        fixed32_mul(dx, sin_val),
        fixed32_mul(dy, cos_val)
    );
    
    result.y = fixed32_add(
        fixed32_mul(dx, cos_val),
        fixed32_mul(dy, sin_val)
    );

    printf("Result view-space: (%d,%d)\n", fixed32_to_int(result.x), fixed32_to_int(result.y));
    
    return result;
}

void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
    t_fixed_vec32 v1 = transform_point(line->start, game);
    t_fixed_vec32 v2 = transform_point(line->end, game);

    debug_after_wall_transform(v1, v2);

    // Apply minimum z-distance to prevent extreme projections
    t_fixed32 z1 = fix_max(v1.y, FIXED_POINT_SCALE * 4);
    t_fixed32 z2 = fix_max(v2.y, FIXED_POINT_SCALE * 4);

    // Scale down projection factor to get reasonable heights
    t_fixed32 projection_scale = int_to_fixed32(WINDOW_HEIGHT / 2);
    
    // Projection calculations with better scaling
    t_fixed32 px1 = fixed32_div(fixed32_mul(v1.x, int_to_fixed32(WINDOW_WIDTH/4)), z1);
    t_fixed32 px2 = fixed32_div(fixed32_mul(v2.x, int_to_fixed32(WINDOW_WIDTH/4)), z2);

    // Convert to screen coordinates
    int x1 = WINDOW_WIDTH/2 + fixed32_to_int(px1);
    int x2 = WINDOW_WIDTH/2 + fixed32_to_int(px2);

    // Better height calculation with scaling
    t_fixed32 h1 = fixed32_div(projection_scale, z1);
    t_fixed32 h2 = fixed32_div(projection_scale, z2);

    // Clipping against screen borders
    if (x1 >= WINDOW_WIDTH || x2 < 0 || x1 >= x2)
        return;

    x1 = fix_max(x1, 0);
    x2 = fix_min(x2, WINDOW_WIDTH - 1);
    
    // Get the appropriate texture for this wall
    t_texture *wall_texture = get_wall_texture(game, line->linedef_index, true, WALL_MIDDLE);
    if (!wall_texture) {
        // Fallback if texture isn't found
        wall_texture = get_wall_texture(game, 0, true, WALL_MIDDLE);  // Try default texture
    }

    // Calculate wall length for texture mapping
    t_fixed32 wall_length = fixed32_vec_dist(line->start, line->end);
    
    // Interpolation and rendering
    for (int x = x1; x <= x2; x++)
    {
        // Use fixed-point for interpolation coefficient
        t_fixed32 dx = int_to_fixed32(x - x1);
        t_fixed32 total_width = int_to_fixed32(x2 - x1 > 0 ? x2 - x1 : 1);
        t_fixed32 t = fixed32_div(dx, total_width);
        
        // Calculate wall height at this point using fixed-point
        t_fixed32 h_fixed = fixed32_add(h1, fixed32_mul(t, fixed32_sub(h2, h1)));
        int h = fixed32_to_int(h_fixed);
        
        // Apply a maximum height limit to prevent extreme values
        h = h > WINDOW_HEIGHT ? WINDOW_HEIGHT : h;
        
        // Calculate screen y-coordinates with proper centering
        int center_y = WINDOW_HEIGHT / 2;
        int top = center_y - h/2;
        int bottom = center_y + h/2;

        // Strong clipping for y-coordinates
        top = top < 0 ? 0 : top;
        bottom = bottom >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : bottom;

        // Clipping against scanline buffer
        if (top < buffer->y_top[x])
            top = buffer->y_top[x];
        if (bottom > buffer->y_bottom[x])
            bottom = buffer->y_bottom[x];

        // Only render if visible
        if (top <= bottom)
        {
            // Calculate depth for shading
            t_fixed32 depth = fixed32_add(v1.y, fixed32_mul(t, fixed32_sub(v2.y, v1.y)));
            
            // Calculate texture x-coordinate based on wall position
            int tex_x;
            if (wall_texture) {
                // Calculate texture position based on distance along the wall
                t_fixed32 tex_pos = fixed32_mul(t, wall_length);
                tex_x = fixed32_to_int(fixed32_mul(tex_pos, int_to_fixed32(wall_texture->width))) % wall_texture->width;
                
                // Fix texture direction based on wall orientation if needed
                if (line->type & 1) { // Assume bit 0 indicates direction
                    tex_x = wall_texture->width - 1 - tex_x;
                }
            }
            
            // Calculate shade based on depth - create a depth scaling factor
            t_fixed32 depth_factor = fixed32_div(int_to_fixed32(10000), depth);
            double shade_factor = fixed32_to_float(fix_min(depth_factor, FIXED_POINT_SCALE)); // Max 1.0
            
            debug_scanline_details(x, top, bottom, h, depth);

            // Render the column
            for (int y = top; y <= bottom; y++)
            {
                unsigned int color;
                
                if (wall_texture) {
                    // Calculate texture y-coordinate
                    t_fixed32 wall_y_pos = fixed32_div(int_to_fixed32(y - top), int_to_fixed32(bottom - top + 1));
                    int tex_y = fixed32_to_int(fixed32_mul(wall_y_pos, int_to_fixed32(wall_texture->height)));
                    tex_y = tex_y % wall_texture->height;
                    
                    // Get pixel from texture
                    color = get_texture_pixel(wall_texture, tex_x, tex_y);
                } else {
                    // Fallback if no texture
                    color = 0xFFFFFF;
                }
                
                // Apply depth-based shading
                color = apply_shade(color, shade_factor);
                
                draw_pixel(game, x, y, color);
            }

            // Update scanline buffer
            buffer->y_top[x] = bottom + 1;
        }
    }
}