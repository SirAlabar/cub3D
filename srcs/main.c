/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/11 13:48:06 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#define MARGIN 50

typedef struct s_bounds {
    float min_x;
    float max_x;
    float min_y;
    float max_y;
} t_bounds;

void draw_point(t_game *game, int x, int y, int color)
{
    int size = 4;
    int i, j;

    for (i = -size; i <= size; i++) {
        for (j = -size; j <= size; j++) {
            if (x + i >= 0 && x + i < WINDOW_WIDTH && 
                y + j >= 0 && y + j < WINDOW_HEIGHT) {
                mlx_pixel_put(game->mlx, game->win, x + i, y + j, color);
            }
        }
    }
}

void calculate_bounds(t_bsp_node *node, t_bounds *bounds, bool first)
{
    if (!node || !node->partition)
        return;

    float start_x = fixed32_to_float(node->partition->start.x);
    float start_y = fixed32_to_float(node->partition->start.y);
    float end_x = fixed32_to_float(node->partition->end.x);
    float end_y = fixed32_to_float(node->partition->end.y);

    if (first) {
        bounds->min_x = fmin(start_x, end_x);
        bounds->max_x = fmax(start_x, end_x);
        bounds->min_y = fmin(start_y, end_y);
        bounds->max_y = fmax(start_y, end_y);
    } else {
        bounds->min_x = fmin(bounds->min_x, fmin(start_x, end_x));
        bounds->max_x = fmax(bounds->max_x, fmax(start_x, end_x));
        bounds->min_y = fmin(bounds->min_y, fmin(start_y, end_y));
        bounds->max_y = fmax(bounds->max_y, fmax(start_y, end_y));
    }

    if (node->front)
        calculate_bounds(node->front, bounds, false);
    if (node->back)
        calculate_bounds(node->back, bounds, false);
}

void draw_line_scaled(t_game *game, t_fixed_vec32 start, t_fixed_vec32 end, int color, t_bounds *bounds, float intensity)
{
    // Prevenir divisão por zero ou valores muito pequenos
    float map_width = bounds->max_x - bounds->min_x;
    float map_height = bounds->max_y - bounds->min_y;
    if (map_width < 0.001f) map_width = 0.001f;
    if (map_height < 0.001f) map_height = 0.001f;

    float scale_x = (WINDOW_WIDTH - 2 * MARGIN) / map_width;
    float scale_y = (WINDOW_HEIGHT - 2 * MARGIN) / map_height;
    float scale = fmin(scale_x, scale_y);

    // Limitar a escala para evitar valores muito grandes
    if (scale > 1000.0f) scale = 1000.0f;

    float offset_x = WINDOW_WIDTH / 2 - ((bounds->max_x + bounds->min_x) / 2) * scale;
    float offset_y = WINDOW_HEIGHT / 2 - ((bounds->max_y + bounds->min_y) / 2) * scale;

    int start_x = (int)(fixed32_to_float(start.x) * scale + offset_x);
    int start_y = (int)(fixed32_to_float(start.y) * scale + offset_y);
    int end_x = (int)(fixed32_to_float(end.x) * scale + offset_x);
    int end_y = (int)(fixed32_to_float(end.y) * scale + offset_y);

    // Verificar coordenadas antes de desenhar
    if (start_x < -WINDOW_WIDTH || start_x > WINDOW_WIDTH * 2 || 
        start_y < -WINDOW_HEIGHT || start_y > WINDOW_HEIGHT * 2 ||
        end_x < -WINDOW_WIDTH || end_x > WINDOW_WIDTH * 2 || 
        end_y < -WINDOW_HEIGHT || end_y > WINDOW_HEIGHT * 2)
        return;

    // Ajustar cor baseado na intensidade
    int r = ((color >> 16) & 0xFF) * intensity;
    int g = ((color >> 8) & 0xFF) * intensity;
    int b = (color & 0xFF) * intensity;
    int adjusted_color = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);

    // Pontos nos vértices
    if (intensity > 0.9f) {
        draw_point(game, start_x, start_y, 0xFFFFFF);
        draw_point(game, end_x, end_y, 0xFFFFFF);
    }

    // Algoritmo de Bresenham
    int dx = abs(end_x - start_x);
    int dy = abs(end_y - start_y);
    int sx = start_x < end_x ? 1 : -1;
    int sy = start_y < end_y ? 1 : -1;
    int err = dx - dy;
    int x = start_x;
    int y = start_y;

    while (1) {
        if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
            mlx_pixel_put(game->mlx, game->win, x, y, adjusted_color);

        if (x == end_x && y == end_y)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

float get_line_intensity(t_bsp_line *line)
{
    return (line->type != 0) ? 1.0f : 0.3f;
}

void render_bsp_tree(t_game *game, t_bsp_node *node, t_bounds *bounds)
{
    if (!node || !node->partition)
        return;

    static int colors[] = {
        0xFF4444, 0x44FF44, 0x4444FF, 
        0xFFFF44, 0xFF44FF, 0x44FFFF
    };
    static int depth = 0;
    int color = colors[depth % 6];

    float intensity = get_line_intensity(node->partition);
    draw_line_scaled(game, node->partition->start, node->partition->end, color, bounds, intensity);

    depth++;
    if (node->front)
        render_bsp_tree(game, node->front, bounds);
    if (node->back)
        render_bsp_tree(game, node->back, bounds);
    depth--;
}

void draw_player(t_game *game, t_bounds *bounds)
{
    float map_width = bounds->max_x - bounds->min_x;
    float map_height = bounds->max_y - bounds->min_y;
    float scale_x = (WINDOW_WIDTH - 2 * MARGIN) / map_width;
    float scale_y = (WINDOW_HEIGHT - 2 * MARGIN) / map_height;
    float scale = fmin(scale_x, scale_y);
    
    // Ajusta o offset para centralizar o mapa
    float offset_x = WINDOW_WIDTH / 2 - ((bounds->max_x + bounds->min_x) / 2) * scale;
    float offset_y = WINDOW_HEIGHT / 2 - ((bounds->max_y + bounds->min_y) / 2) * scale;

    // Calcula a posição do player usando a mesma transformação que usamos para as paredes
    // Posição hardcoded do player (5,5)
    game->p1.pos.x = float_to_fixed32(5.0f);
    game->p1.pos.y = float_to_fixed32(5.0f);
    
    float player_pos_x = fixed32_to_float(game->p1.pos.x);
    float player_pos_y = fixed32_to_float(game->p1.pos.y);
    
    // Aplica a mesma transformação que usamos para os vértices do mapa
    int player_x = (int)(player_pos_x * scale + offset_x);
    int player_y = (int)(player_pos_y * scale + offset_y);

    // Debug info
    printf("Player map pos: (%f, %f)\n", player_pos_x, player_pos_y);
    printf("Player screen pos: (%d, %d)\n", player_x, player_y);
    printf("Scale: %f, Offset: (%f, %f)\n", scale, offset_x, offset_y);

    // Desenhar círculo para o player
    int radius = 6;
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx*dx + dy*dy <= radius*radius) {
                int draw_x = player_x + dx;
                int draw_y = player_y + dy;
                if (draw_x >= 0 && draw_x < WINDOW_WIDTH && draw_y >= 0 && draw_y < WINDOW_HEIGHT) {
                    mlx_pixel_put(game->mlx, game->win, draw_x, draw_y, 0xFF0000);  // Vermelho
                }
            }
        }
    }

    // Desenhar direção do player (seta)
    int direction_length = 20;
    int dir_x = player_x + direction_length * fixed32_to_float(game->p1.dir.x);
    int dir_y = player_y + direction_length * fixed32_to_float(game->p1.dir.y);

    // Linha da direção
    draw_line_scaled(game, 
        (t_fixed_vec32){float_to_fixed32(player_x), float_to_fixed32(player_y)},
        (t_fixed_vec32){float_to_fixed32(dir_x), float_to_fixed32(dir_y)},
        0xFF0000, bounds, 1.0f);
}

int render_frame(t_game *game)
{
    mlx_clear_window(game->mlx, game->win);

    if (game->bsp_tree && game->bsp_tree->root) {
        t_bounds bounds;
        calculate_bounds(game->bsp_tree->root, &bounds, true);
        render_bsp_tree(game, game->bsp_tree->root, &bounds);
        draw_player(game, &bounds);
    }

    return (0);
}

int main(int argc, char **argv)
{
    t_game      *game;
    t_doom_map  *map;
    t_bsp_tree  *tree;

    game = malloc(sizeof(t_game));
    ft_bzero(game, sizeof(t_game));
    if (!game)
        return (1);

    // Carregar o mapa a partir de um arquivo
    map = malloc(sizeof(t_doom_map));
    if (!map)
        return (1);
    if (!load_map(argc, argv, map))
    {
        free(map);
        return (1);
    }

    // Construir a árvore BSP a partir do mapa
    tree = init_bsp_build(map);
    if (!tree)
    {
        free(map);
        return (1);
    }

    // Inicializar a janela do jogo
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "BSP Visualization");
    if (!game->win)
    {
        //cleanup_game(game);
        free_bsp_tree(tree);
        free(map);
        return (1);
    }

    // Atribuir o mapa e a árvore BSP ao jogo
    game->map = map;
    game->bsp_tree = tree;

    // Iniciar o loop do jogo
    mlx_loop_hook(game->mlx, &render_frame, game);
    mlx_loop(game->mlx);

    // Liberar recursos
    //cleanup_game(game);
    free_bsp_tree(tree);
    free(map);

    return (0);
}

