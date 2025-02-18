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
#define DOOM_UNITS_PER_TILE 64    // Unidades DOOM por tile
#define SCALE_FACTOR        4
#define FOV_ANGLE          90     // Campo de visão em graus
#define FOV_SCALE          (WINDOW_WIDTH / 2)  

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
    printf("Player angle (raw): %d (deg): %f\n",
           game->p1.angle, (float)game->p1.angle * 360.0f / ANG360);
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
	printf("FOV factor (fixed): %d (int): %d\n", 
			DOOM_UNITS_PER_TILE, fixed32_to_int(DOOM_UNITS_PER_TILE));
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

void debug_after_wall_projection(int x1, int x2, int h1, int h2) {
    if (!g_debug.enabled) return;
   
    g_debug.wall_debug.x1_screen = x1;
    g_debug.wall_debug.x2_screen = x2;
    g_debug.wall_debug.h1_screen = h1;
    g_debug.wall_debug.h2_screen = h2;
   
    printf("Screen space coordinates:\n");
    printf("X range: %d to %d\n", x1, x2);
    printf("Heights: start=%d end=%d\n", h1, h2);
}

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

static t_fixed32 clamp(t_fixed32 val, t_fixed32 min, t_fixed32 max) {
    if (val <= min) return min;
    if (val >= max) return max;
    return val;
}

t_fixed_vec32 transform_point(t_fixed_vec32 p, t_game *game)
{
    t_fixed_vec32 result;
    
    printf("\n=== Transform Point Debug ===\n");
    printf("Input point - fixed: (%d,%d) int: (%d,%d)\n", 
        p.x, p.y, fixed32_to_int(p.x), fixed32_to_int(p.y));
    printf("Player pos - fixed: (%d,%d) int: (%d,%d)\n",
        game->p1.pos.x, game->p1.pos.y, 
        fixed32_to_int(game->p1.pos.x), fixed32_to_int(game->p1.pos.y));

    t_fixed32 dx = fixed32_sub(p.x, game->p1.pos.x);
    t_fixed32 dy = fixed32_sub(p.y, game->p1.pos.y);

    printf("Delta - fixed: (%d,%d) int: (%d,%d)\n", 
        dx, dy, fixed32_to_int(dx), fixed32_to_int(dy));

    unsigned int angle_index = (game->p1.angle >> ANGLETOFINESHIFT) & FINEMASK;
    t_fixed32 cos_val = get_cos_8192(game->fixed_tables, angle_index);
    t_fixed32 sin_val = get_sin_8192(game->fixed_tables, angle_index);
    
    printf("Angle: %d, cos: %d, sin: %d\n", 
        angle_index, fixed32_to_int(cos_val), fixed32_to_int(sin_val));

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


// Em scanline_rendering.c:
void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
    printf("\n=== Render Wall Segment Debug ===\n");
    printf("Wall from - fixed: (%d,%d) to (%d,%d)\n",
        line->start.x, line->start.y, line->end.x, line->end.y);

    t_fixed_vec32 v1 = transform_point(line->start, game);
    t_fixed_vec32 v2 = transform_point(line->end, game);

    printf("Transformed - v1: (%d,%d) v2: (%d,%d)\n",
        fixed32_to_int(v1.x), fixed32_to_int(v1.y),
        fixed32_to_int(v2.x), fixed32_to_int(v2.y));

    // Early frustum culling
	if (v1.y <= (FIXED_POINT_SCALE >> 8) || v2.y <= (FIXED_POINT_SCALE >> 8))
	{
		printf("Wall culled - too close to camera (y1=%d, y2=%d)\n", 
			fixed32_to_int(v1.y), fixed32_to_int(v2.y));
		return;
	}

    // Projeta para coordenadas de tela
    int x1 = WINDOW_WIDTH/2 + fixed32_to_int(
        fixed32_div(fixed32_mul(v1.x, int_to_fixed32(FOV_SCALE)),
        v1.y)
    );
    
    int x2 = WINDOW_WIDTH/2 + fixed32_to_int(
        fixed32_div(fixed32_mul(v2.x, int_to_fixed32(FOV_SCALE)),
        v2.y)
    );

    printf("Screen X coordinates: x1=%d x2=%d\n", x1, x2);

    // Calcula alturas usando a escala DOOM correta
    int h1 = fixed32_to_int(
        fixed32_div(
            fixed32_mul(int_to_fixed32(WALL_HEIGHT_SCALE), FIXED_POINT_SCALE),
            v1.y
        )
    );
    
    int h2 = fixed32_to_int(
        fixed32_div(
            fixed32_mul(int_to_fixed32(WALL_HEIGHT_SCALE), FIXED_POINT_SCALE),
            v2.y
        )
    );

    printf("Wall heights: h1=%d h2=%d\n", h1, h2);

    // Clipping e renderização
    if (x1 >= WINDOW_WIDTH || x2 < 0) {
        printf("Wall culled - outside screen\n");
        return;
    }
        
    x1 = clamp(x1, 0, WINDOW_WIDTH-1);
    x2 = clamp(x2, 0, WINDOW_WIDTH-1);

    printf("After clipping - x1=%d x2=%d\n", x1, x2);

    // Para cada coluna da parede
    for (int x = x1; x <= x2; x++)
    {
        // Interpola altura
        float t = (float)(x - x1) / (x2 - x1);
		int h = fixed32_to_int(
			fixed32_div(
				fixed32_mul(int_to_fixed32(WALL_HEIGHT_SCALE), 
						   int_to_fixed32(WINDOW_HEIGHT / 2)),
				abs(v1.y)  // Usar valor absoluto aqui
			)
		);
        
        // Calcula topo e base considerando altura do jogador
        int center_y = WINDOW_HEIGHT/2 - fixed32_to_int(
            fixed32_mul(game->p1.view_z - game->p1.sector->floor_height,
            int_to_fixed32(WINDOW_HEIGHT) / WALL_HEIGHT_SCALE)
        );
        
        int top = center_y - (h >> 1);
        int bottom = center_y + (h >> 1);

        if (x % 100 == 0) { // Debug apenas a cada 100 pixels para não sobrecarregar
            printf("Column x=%d - height=%d center_y=%d top=%d bottom=%d\n", 
                x, h, center_y, top, bottom);
        }

        // Clipping vertical
        if (top < buffer->y_top[x])
            top = buffer->y_top[x];
        if (bottom > buffer->y_bottom[x])
            bottom = buffer->y_bottom[x];

        // Calcula profundidade para shading
        t_fixed32 depth = fixed32_mul(
            fixed32_add(
                v1.y,
                fixed32_mul(
                    fixed32_sub(v2.y, v1.y),
                    float_to_fixed32(t)
                )
            ),
            int_to_fixed32(SCALE_FACTOR)
        );

        // Calcula shading baseado na distância
        int shade = fixed32_to_int(
            fixed32_div(FIXED_POINT_SCALE << 6, depth)
        );
        shade = clamp(shade, 32, 255);

        if (x % 100 == 0) {
            printf("Column x=%d - depth=%d shade=%d\n", 
                x, fixed32_to_int(depth), shade);
        }

        // Desenha a coluna da parede
        for (int y = top; y <= bottom; y++)
        {
            // Adiciona gradiente vertical
            float fy = (float)(y - top) / (bottom - top);
            int gradient_shade = (int)(shade * (1.0f - fy * 0.3f));
            gradient_shade = clamp(gradient_shade, 32, 255);
            
            int color = (gradient_shade << 16) | 
                       (gradient_shade << 8) | 
                       gradient_shade;
                       
            draw_pixel(game, x, y, color);
        }

        // Atualiza buffer de scanline
        buffer->y_top[x] = bottom + 1;
    }
    printf("=== End Wall Segment ===\n\n");
}