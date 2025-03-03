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
#define DEBUG_SCALE 0.25          // Scale factor for map rendering
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


// static bool is_line_visible(t_game *game, t_bsp_line *line)
// {
//     t_fixed_vec32 to_start, to_end;
//     t_fixed32 dot_start, dot_end;
//     t_fixed32 angle_cos, angle_sin;
    
//     // Vector from player to line endpoints
//     to_start.x = fixed32_sub(line->start.x, game->p1.pos.x);
//     to_start.y = fixed32_sub(line->start.y, game->p1.pos.y);
//     to_end.x = fixed32_sub(line->end.x, game->p1.pos.x);
//     to_end.y = fixed32_sub(line->end.y, game->p1.pos.y);
    
//     // Get player's viewing direction using lookup tables
//     angle_cos = get_cos_8192(game->fixed_tables, game->p1.angle);
//     angle_sin = get_sin_8192(game->fixed_tables, game->p1.angle);
        
//     // ft_printf("Visibility check - angle: %d, cos: %d, sin: %d\n",
//     //     (game->p1.angle >> ANGLETOFINESHIFT) & FINEMASK,
//     //     fixed32_to_int(angle_cos),
//     //     fixed32_to_int(angle_sin));
    
//     // Calculate dot products
//     dot_start = fixed32_add(
//         fixed32_mul(to_start.x, angle_cos),
//         fixed32_mul(to_start.y, angle_sin)
//     );
//     dot_end = fixed32_add(
//         fixed32_mul(to_end.x, angle_cos),
//         fixed32_mul(to_end.y, angle_sin)
//     );
    
//     // Line is potentially visible if at least one endpoint is in front
//     return (dot_start > 0 || dot_end > 0);
// }


static void draw_bsp_line(t_game *game, t_bsp_line *line, int color)
{
    t_vector_i start = world_to_screen(line->start);
    t_vector_i end = world_to_screen(line->end);

    draw_line(game, start, end, color);
}



// static void draw_bsp_node(t_game *game, t_bsp_node *node)
// {
//     if (!node)
//         return;

//     // Draw lines in this node
//     for (int i = 0; i < node->num_lines; i++)
//     {
//         if (node->lines[i])
//             draw_bsp_line(game, node->lines[i]);
//     }

//     // Draw partition line if it exists
//     if (node->partition)
//         draw_bsp_line(game, node->partition);

//     // Recursively draw child nodes
//     if (node->front)
//         draw_bsp_node(game, node->front);
//     if (node->back)
//         draw_bsp_node(game, node->back);
// }


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


void debug_print_bsp_tree_details(t_bsp_node *node, int depth)
{
    int i;

    // Indent based on depth
    for (i = 0; i < depth; i++)
        printf("  ");
    
    printf("BSP Node at Depth %d:\n", depth);

    // Indent for node details
    for (i = 0; i < depth + 1; i++)
        printf("  ");
    
    // Adiciona verificações de segurança
    if (!node)
    {
        printf("NULL Node\n");
        return;
    }

    printf("Num Lines: %d\n", node->num_lines);

    // Verificação de segurança para linhas
    if (node->num_lines > 0)
    {
        if (node->lines)
        {
            for (i = 0; i < node->num_lines; i++)
            {
                // Verificação adicional para cada linha
                if (node->lines[i])
                {
                    for (int j = 0; j < depth + 2; j++)
                        printf("  ");
                    // if(node->lines[i])
                    //     {printf("Line %d: (%d,%d) -> (%d,%d)\n", 
                    //     i,
                    //     fixed32_to_int(node->lines[i]->start.x), 
                    //     fixed32_to_int(node->lines[i]->start.y),
                    //     fixed32_to_int(node->lines[i]->end.x), 
                    //     fixed32_to_int(node->lines[i]->end.y));}
                }
                else
                {
                    for (int j = 0; j < depth + 2; j++)
                        printf("  ");
                    printf("Line %d: NULL pointer\n", i);
                }
            }
        }
        else
        {
            // Se num_lines > 0 mas lines é NULL, isso é um erro
            for (i = 0; i < depth + 1; i++)
                printf("  ");
            printf("ERROR: num_lines = %d but lines is NULL\n", node->num_lines);
        }
    }
    else
    {
        for (i = 0; i < depth + 1; i++)
            printf("  ");
        printf("No Lines\n");
    }
    if (node->partition)
    {
        for (i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Partition Line: (%d,%d) -> (%d,%d)\n", 
            fixed32_to_int(node->partition->start.x), 
            fixed32_to_int(node->partition->start.y),
            fixed32_to_int(node->partition->end.x), 
            fixed32_to_int(node->partition->end.y));
    }
    else
    {
        for (i = 0; i < depth + 1; i++)
            printf("  ");
        printf("No Partition Line\n");
    }

    // Recursivamente print child nodes
    if (node->front)
    {
        for (i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Front Child:\n");
        debug_print_bsp_tree_details(node->front, depth + 1);
    }

    if (node->back)
    {
        for (i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Back Child:\n");
        debug_print_bsp_tree_details(node->back, depth + 1);
    }
}

// Simple wrapper to add a header and footer to the debug print
void print_detailed_bsp_tree(t_bsp_node *root)
{
    printf("\n===== BSP Tree Detailed Debug =====\n");
    if (!root)
    {
        printf("Empty BSP Tree\n");
        return;
    }
    debug_print_bsp_tree_details(root, 0);
    printf("===== End of BSP Tree Debug =====\n\n");
}

int render_frame(t_game *game)
{
    ft_printf("\n=== Starting Debug Render Frame ===\n");

    // Inicializar estrutura de linhas visíveis
    t_visible_lines visible_lines;
    visible_lines.count = 0;
    print_detailed_bsp_tree(game->bsp_tree->root);
    // Limpar o buffer
    draw_background(game);
    
    // Desenhar grid primeiro para ficar atrás de tudo
    draw_grid(game);
    
    // Usar a travessia BSP existente para determinar linhas visíveis
    if (game->bsp_tree && game->bsp_tree->root)
    {
        // Encontrar linhas visíveis usando sua função de travessia
        mark_visible_bsp_lines(game->bsp_tree->root, game, &visible_lines);
        
        // Criar um array para rastrear quais linhas são visíveis
        bool *is_visible = calloc(MAX_LINEDEFS, sizeof(bool));
        if (is_visible)
        {
            // Marcar quais linhas são visíveis
            for (int i = 0; i < visible_lines.count; i++)
            {
                int linedef_index = visible_lines.line_ids[i];
                is_visible[linedef_index] = true;
            }
            
            // Função de processamento para desenhar nós
            // Esta será passada para sua função de travessia
            void draw_node_lines(t_bsp_node *node)
            {
                // Desenhar linhas deste nó
                for (int i = 0; i < node->num_lines; i++)
                {
                    if (node->lines[i])
                    {
                        int color = is_visible[node->lines[i]->linedef_index] ? 
                                    DEBUG_WALL_VISIBLE : DEBUG_WALL_HIDDEN;
                        draw_bsp_line(game, node->lines[i], color);
                    }
                }
                
                // Desenhar a partição
                if (node->partition)
                {
                    int color = is_visible[node->partition->linedef_index] ? 
                                DEBUG_WALL_VISIBLE : DEBUG_WALL_HIDDEN;
                    draw_bsp_line(game, node->partition, color);
                }
            }
            
            // Usar sua função de travessia existente com nossa função de desenho
            traverse_front_to_back(game->bsp_tree->root, game->p1.pos, draw_node_lines);
            
            free(is_visible);
        }
        else
        {
            // Fallback caso a alocação falhe
            for (int i = 0; i < visible_lines.count; i++)
            {
                int linedef_index = visible_lines.line_ids[i];
                t_bsp_line *line = find_line_by_linedef_index(game->bsp_tree->root, linedef_index);
                if (line)
                    draw_bsp_line(game, line, DEBUG_WALL_VISIBLE);
            }
        }
    }
    else
    {
        ft_printf("Warning: No BSP tree to render!\n");
    }

    // Desenhar o jogador por último para sempre ficar visível
    draw_player(game);

    // Tratar movimento do jogador
    move_player(game);

    // Trocar buffers
    swap_buffers(game);

    ft_printf("=== Debug Render Frame Complete ===\n\n");
    return (0);
}