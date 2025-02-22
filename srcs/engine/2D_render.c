/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_debug_2d.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/21 18:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

// Colors for debug rendering
#define DEBUG_BACKGROUND 0x000000  // Black
#define DEBUG_WALL_HIDDEN 0x006400 // Dark green
#define DEBUG_WALL_VISIBLE 0xFF0000 // Pure red
#define DEBUG_PLAYER 0x0000FF     // Blue
#define DEBUG_SCALE 0.65          // Scale factor for map rendering
#define DIRECTION_INDICATOR_LENGTH (PLAYER_RADIUS * 3)  // Length of player direction line
#define DEBUG_GRID_COLOR 0x1A1A1A  // Very dark gray for grid


void draw_background(t_game *game)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            draw_pixel(game, x, y, DEBUG_BACKGROUND);
        }
    }
}


static t_vector_i world_to_screen(t_fixed_vec32 world_pos)
{
    t_vector_i screen_pos;
    double scale = DEBUG_SCALE;
    
    screen_pos.x = (WINDOW_WIDTH / 2) + (int)(fixed32_to_float(world_pos.x) * scale);
    screen_pos.y = (WINDOW_HEIGHT / 2) + (int)(fixed32_to_float(world_pos.y) * scale);
    
    // ft_printf("World pos (%d,%d) -> Screen pos (%d,%d)\n",
    //     fixed32_to_int(world_pos.x), fixed32_to_int(world_pos.y),
    //     screen_pos.x, screen_pos.y);
    
    return screen_pos;
}

static void draw_line(t_game *game, t_vector_i start, t_vector_i end, int color)
{
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;
    t_vector_i curr = start;

    // ft_printf("Drawing line from (%d,%d) to (%d,%d) with color 0x%06X\n",
    //     start.x, start.y, end.x, end.y, color);

    while (true)
    {
        if (curr.x >= 0 && curr.x < WINDOW_WIDTH && 
            curr.y >= 0 && curr.y < WINDOW_HEIGHT)
            draw_pixel(game, curr.x, curr.y, color);
            
        if (curr.x == end.x && curr.y == end.y)
            break;
            
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            curr.x += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            curr.y += sy;
        }
    }
}


static void draw_player(t_game *game)
{
    t_vector_i player_pos = world_to_screen(game->p1.pos);
    const int radius = 6; // Slightly larger radius
    int x, y;

   // ft_printf("Drawing player at screen pos (%d,%d)\n", player_pos.x, player_pos.y);

    // Draw player circle with border
    for (y = -radius-1; y <= radius+1; y++)
    {
        for (x = -radius-1; x <= radius+1; x++)
        {
            if (x*x + y*y <= (radius+1)*(radius+1))
            {
                // Black border
                draw_pixel(game, player_pos.x + x, player_pos.y + y, DEBUG_BACKGROUND);
            }
        }
    }
    
    // Fill player circle
    for (y = -radius; y <= radius; y++)
    {
        for (x = -radius; x <= radius; x++)
        {
            if (x*x + y*y <= radius*radius)
            {
                draw_pixel(game, player_pos.x + x, player_pos.y + y, DEBUG_PLAYER);
            }
        }
    }

    // Draw FOV lines
    t_fixed32 fov_half = FOV / 2;  // FOV/2 em BAM
    t_fixed32 base_angle = game->p1.angle;
    
    // Calculate left and right FOV angles
    t_fixed32 left_angle = (base_angle - fov_half) & ANGLEMASK;   // Usando ANGLEMASK para BAM
    t_fixed32 right_angle = (base_angle + fov_half) & ANGLEMASK; 
    
    // ft_printf("FOV angles - base: %d, left: %d, right: %d\n", 
    //     base_angle, left_angle, right_angle);
    
    // Calculate FOV line endpoints
    t_fixed_vec32 left_dir, right_dir;
    t_vector_i left_end, right_end;
    
    // Left FOV line
    left_dir.x = fixed32_mul(int_to_fixed32(DIRECTION_INDICATOR_LENGTH * 2), 
        get_cos_8192(game->fixed_tables, left_angle));
    left_dir.y = fixed32_mul(int_to_fixed32(DIRECTION_INDICATOR_LENGTH * 2), 
        get_sin_8192(game->fixed_tables, left_angle));
    left_end.x = player_pos.x + fixed32_to_int(left_dir.x);
    left_end.y = player_pos.y + fixed32_to_int(left_dir.y);
    
    // Right FOV line
    right_dir.x = fixed32_mul(int_to_fixed32(DIRECTION_INDICATOR_LENGTH * 2), 
        get_cos_8192(game->fixed_tables, right_angle));
    right_dir.y = fixed32_mul(int_to_fixed32(DIRECTION_INDICATOR_LENGTH * 2), 
        get_sin_8192(game->fixed_tables, right_angle));
    right_end.x = player_pos.x + fixed32_to_int(right_dir.x);
    right_end.y = player_pos.y + fixed32_to_int(right_dir.y);
    
    // ft_printf("Drawing FOV lines from (%d,%d) to L(%d,%d) and R(%d,%d)\n",
    //     player_pos.x, player_pos.y, left_end.x, left_end.y, right_end.x, right_end.y);
    
    // Draw the two FOV lines
    draw_line(game, player_pos, left_end, DEBUG_WALL_VISIBLE);  // Red for better visibility
    draw_line(game, player_pos, right_end, DEBUG_WALL_VISIBLE); // Red for better visibility
}


static bool is_line_visible(t_game *game, t_bsp_line *line)
{
    t_fixed_vec32 to_start, to_end;
    t_fixed32 dot_start, dot_end;
    t_fixed32 angle_cos, angle_sin;
    
    // Vector from player to line endpoints
    to_start.x = fixed32_sub(line->start.x, game->p1.pos.x);
    to_start.y = fixed32_sub(line->start.y, game->p1.pos.y);
    to_end.x = fixed32_sub(line->end.x, game->p1.pos.x);
    to_end.y = fixed32_sub(line->end.y, game->p1.pos.y);
    
    // Get player's viewing direction using lookup tables
    angle_cos = get_cos_8192(game->fixed_tables, game->p1.angle);
    angle_sin = get_sin_8192(game->fixed_tables, game->p1.angle);
        
    // ft_printf("Visibility check - angle: %d, cos: %d, sin: %d\n",
    //     (game->p1.angle >> ANGLETOFINESHIFT) & FINEMASK,
    //     fixed32_to_int(angle_cos),
    //     fixed32_to_int(angle_sin));
    
    // Calculate dot products
    dot_start = fixed32_add(
        fixed32_mul(to_start.x, angle_cos),
        fixed32_mul(to_start.y, angle_sin)
    );
    dot_end = fixed32_add(
        fixed32_mul(to_end.x, angle_cos),
        fixed32_mul(to_end.y, angle_sin)
    );
    
    // Line is potentially visible if at least one endpoint is in front
    return (dot_start > 0 || dot_end > 0);
}


static void draw_bsp_line(t_game *game, t_bsp_line *line)
{
    t_vector_i start = world_to_screen(line->start);
    t_vector_i end = world_to_screen(line->end);
    int color;

    bool visible = is_line_visible(game, line);
    color = visible ? DEBUG_WALL_VISIBLE : DEBUG_WALL_HIDDEN;

        // ft_printf("Drawing BSP line: (%d,%d) -> (%d,%d) [%s]\n",
        // fixed32_to_int(line->start.x), fixed32_to_int(line->start.y),
        // fixed32_to_int(line->end.x), fixed32_to_int(line->end.y),
        // visible ? "visible" : "hidden");

    draw_line(game, start, end, color);
}


static void draw_bsp_node(t_game *game, t_bsp_node *node)
{
    if (!node)
        return;

    // Draw lines in this node
    for (int i = 0; i < node->num_lines; i++)
    {
        if (node->lines[i])
            draw_bsp_line(game, node->lines[i]);
    }

    // Draw partition line if it exists
    if (node->partition)
        draw_bsp_line(game, node->partition);

    // Recursively draw child nodes
    if (node->front)
        draw_bsp_node(game, node->front);
    if (node->back)
        draw_bsp_node(game, node->back);
}


static void get_map_bounds(t_doom_map *map, t_fixed32 *min_x, t_fixed32 *max_x, 
                         t_fixed32 *min_y, t_fixed32 *max_y)
{
    *min_x = map->vertices[0].pos.x;
    *max_x = map->vertices[0].pos.x;
    *min_y = map->vertices[0].pos.y;
    *max_y = map->vertices[0].pos.y;

    for (int i = 1; i < map->vertex_count; i++)
    {
        if (map->vertices[i].pos.x < *min_x) *min_x = map->vertices[i].pos.x;
        if (map->vertices[i].pos.x > *max_x) *max_x = map->vertices[i].pos.x;
        if (map->vertices[i].pos.y < *min_y) *min_y = map->vertices[i].pos.y;
        if (map->vertices[i].pos.y > *max_y) *max_y = map->vertices[i].pos.y;
    }

//    ft_printf("Map bounds: X(%d to %d) Y(%d to %d)\n",
//        fixed32_to_int(*min_x), fixed32_to_int(*max_x),
 //       fixed32_to_int(*min_y), fixed32_to_int(*max_y));
}


static void draw_grid(t_game *game)
{
    t_fixed32 min_x, max_x, min_y, max_y;
    get_map_bounds(game->map, &min_x, &max_x, &min_y, &max_y);

    // Convert to tile coordinates and add one tile padding
    int start_tile_x = fixed32_to_int(min_x) / TILE_SIZE - 1;
    int end_tile_x = fixed32_to_int(max_x) / TILE_SIZE + 1;
    int start_tile_y = fixed32_to_int(min_y) / TILE_SIZE - 1;
    int end_tile_y = fixed32_to_int(max_y) / TILE_SIZE + 1;
    
  //  int tile_pixels = (int)(TILE_SIZE * DEBUG_SCALE);
    
//    ft_printf("Grid tiles: X(%d to %d) Y(%d to %d), Tile size: %d pixels\n",
//        start_tile_x, end_tile_x, start_tile_y, end_tile_y, tile_pixels);

    // Draw vertical grid lines
    for (int x = start_tile_x; x <= end_tile_x; x++)
    {
        t_fixed_vec32 world_start = {int_to_fixed32(x * TILE_SIZE), min_y};
        t_fixed_vec32 world_end = {int_to_fixed32(x * TILE_SIZE), max_y};
        t_vector_i screen_start = world_to_screen(world_start);
        t_vector_i screen_end = world_to_screen(world_end);
        draw_line(game, screen_start, screen_end, DEBUG_GRID_COLOR);
    }

    // Draw horizontal grid lines
    for (int y = start_tile_y; y <= end_tile_y; y++)
    {
        t_fixed_vec32 world_start = {min_x, int_to_fixed32(y * TILE_SIZE)};
        t_fixed_vec32 world_end = {max_x, int_to_fixed32(y * TILE_SIZE)};
        t_vector_i screen_start = world_to_screen(world_start);
        t_vector_i screen_end = world_to_screen(world_end);
        draw_line(game, screen_start, screen_end, DEBUG_GRID_COLOR);
    }

    // Draw center lines
    t_fixed_vec32 center_world = {0, 0};
    t_vector_i center_screen = world_to_screen(center_world);
    
    t_vector_i center_h1 = {0, center_screen.y};
    t_vector_i center_h2 = {WINDOW_WIDTH, center_screen.y};
    t_vector_i center_v1 = {center_screen.x, 0};
    t_vector_i center_v2 = {center_screen.x, WINDOW_HEIGHT};
    
    // Draw center lines with slightly lighter color
    int center_line_color = DEBUG_GRID_COLOR * 2;  // Multiplica por 2 para ficar mais claro
    draw_line(game, center_h1, center_h2, center_line_color);
    draw_line(game, center_v1, center_v2, center_line_color);
}


int render_frame(t_game *game)
{
    ft_printf("\nplayer position %d, e %d:", game->p1.pos.x, game->p1.pos.y);
    ft_printf("\n=== Starting Debug Render Frame ===\n");

    // Clear the buffer
    draw_background(game);
    
    // Draw grid first so it's behind everything
    draw_grid(game);
    
    // Draw BSP tree if it exists
    if (game->bsp_tree && game->bsp_tree->root)
    {
       // ft_printf("Drawing BSP tree...\n");
        draw_bsp_node(game, game->bsp_tree->root);
    }
    else
    {
        ft_printf("Warning: No BSP tree to render!\n");
    }

    // Draw player last so it's always visible
    draw_player(game);

    // Handle player movement
    move_player(game);

    // Swap buffers
    swap_buffers(game);

    ft_printf("=== Debug Render Frame Complete ===\n\n");
    return (0);
}