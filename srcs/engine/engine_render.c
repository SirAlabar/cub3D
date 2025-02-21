/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/14 19:30:22 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void init_double_buffer(t_game *game)
{
    game->buffer[0] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->buffer[1] = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->buffer[0] || !game->buffer[1])
    {
        ft_putendl_fd("Error: Failed to create rendering buffers", 2);
        cleanup_game(game);
        exit(1);
    }
    game->addr[0] = mlx_get_data_addr(game->buffer[0], 
        &game->bits_per_pixel, &game->line_length, &game->endian);
    game->addr[1] = mlx_get_data_addr(game->buffer[1], 
        &game->bits_per_pixel, &game->line_length, &game->endian);
    game->current_buffer = 0;
}

void swap_buffers(t_game *game)
{
    mlx_put_image_to_window(game->mlx, game->win, 
        game->buffer[game->current_buffer], 0, 0);
    game->current_buffer = !game->current_buffer;
}

void cleanup_double_buffer(t_game *game)
{
    if (game->buffer[0])
        mlx_destroy_image(game->mlx, game->buffer[0]);
    if (game->buffer[1])
        mlx_destroy_image(game->mlx, game->buffer[1]);
    game->buffer[0] = NULL;
    game->buffer[1] = NULL;
}

void draw_background(t_game *game)
{
    int pixel_pos;
    int y;
    int x;

    if (!game || !game->addr[game->current_buffer])
        return;
    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        x = 0;
        while (x < WINDOW_WIDTH)
        {
            pixel_pos = (y * game->line_length) + (x * (game->bits_per_pixel / 8));
            *(unsigned int *)(game->addr[game->current_buffer] + pixel_pos) = 0x00FFFF;
            x++;
        }
        y++;
    }
}

void debug_bsp_render(t_bsp_node *node, int depth) {
    int i;
    
    printf("\n=== BSP Node at depth %d ===\n", depth);
    
    if (!node) {
        printf("Null node\n");
        return;
    }

    // Imprimir informações do nó
    printf("Number of lines: %d\n", node->num_lines);
    
    // Se há uma partição, imprimir seus detalhes
    if (node->partition) {
        printf("Partition line: (%d,%d) -> (%d,%d)\n",
            fixed32_to_int(node->partition->start.x),
            fixed32_to_int(node->partition->start.y),
            fixed32_to_int(node->partition->end.x),
            fixed32_to_int(node->partition->end.y));
    }

    // Imprimir todas as linhas do nó
    printf("Lines in node:\n");
    for (i = 0; i < node->num_lines; i++) {
        if (node->lines[i]) {
            printf("  Line %d: (%d,%d) -> (%d,%d)\n", i,
                fixed32_to_int(node->lines[i]->start.x),
                fixed32_to_int(node->lines[i]->start.y),
                fixed32_to_int(node->lines[i]->end.x),
                fixed32_to_int(node->lines[i]->end.y));
        }
    }

    // Recursive debug para nós filhos
    if (node->front) {
        printf("\nFront child at depth %d:\n", depth + 1);
        debug_bsp_render(node->front, depth + 1);
    }
    if (node->back) {
        printf("\nBack child at depth %d:\n", depth + 1);
        debug_bsp_render(node->back, depth + 1);
    }
}
void render_bsp_node(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
    t_bsp_side side;
    int i;

    if (!node)
        return;

    debug_bsp_render(node, 0);  // Adicionar aqui o debug

    // Se não é um nó folha, usa a partição para determinar a ordem
    if (node->partition)
    {
        side = bsp_classify_point(game->p1.pos, node->partition);
        printf("Player classified as %s relative to partition\n",
            side == BSP_FRONT ? "FRONT" :
            side == BSP_BACK ? "BACK" :
            side == BSP_COLINEAR ? "COLINEAR" : "SPANNING");
        
        // Renderiza de trás pra frente
        if (side == BSP_FRONT)
        {
            render_bsp_node(game, node->back, buffer);
            
            // Renderiza linhas do nó atual
            for (i = 0; i < node->num_lines; i++)
                if (node->lines[i]) {
                    printf("Rendering line %d in FRONT node\n", i);
                    render_wall_segment(game, node->lines[i], buffer);
                }
            
            render_bsp_node(game, node->front, buffer);
        }
        else // BSP_BACK ou BSP_COLINEAR
        {
            render_bsp_node(game, node->front, buffer);
            
            // Renderiza linhas do nó atual
            for (i = 0; i < node->num_lines; i++)
                if (node->lines[i]) {
                    printf("Rendering line %d in BACK node\n", i);
                    render_wall_segment(game, node->lines[i], buffer);
                }
            
            render_bsp_node(game, node->back, buffer);
        }
    }
    // Nó folha - apenas renderiza suas linhas
    else
    {
        printf("Rendering leaf node with %d lines\n", node->num_lines);
        for (i = 0; i < node->num_lines; i++)
            if (node->lines[i])
                render_wall_segment(game, node->lines[i], buffer);
    }
}
void draw_skybox(t_game *game)
{
    int x, y;
    t_fixed32 angle;
    unsigned int current_angle;
    float pixel_per_degree;
   
    pixel_per_degree = 1024.0f / 360.0f;
    angle = game->p1.angle & ANGLEMASK;
    current_angle = ((unsigned int)angle * 360) >> FIXED_POINT_BITS;
   
    y = -1;
    while (++y < WINDOW_HEIGHT)
    {
        x = -1;
        while (++x < WINDOW_WIDTH)
        {
            float screen_x_ratio = (float)x / WINDOW_WIDTH;
            float angle_offset = (screen_x_ratio - 0.5f) * FOV;
           
            // Garante que o ângulo_offset está nos limites
            angle_offset = fminf(fmaxf(angle_offset, -FOV/2), FOV/2);
           
            if (fabs(angle_offset) <= FOV / 2)
            {
                // Calcula o ângulo total
                float total_angle = current_angle + angle_offset;
                
                // Garante que total_angle sempre esteja entre 0 e 360
                total_angle = fmodf(total_angle + 360.0f, 360.0f);
                
                // Calcula a coordenada X da textura
                float tex_x_float = total_angle * pixel_per_degree;
               
                // Usa floor para garantir mapeamento contínuo
                unsigned int tex_x = (unsigned int)floor(tex_x_float) % game->skybox_tex->width;
                unsigned int tex_y = (y * game->skybox_tex->height) / WINDOW_HEIGHT;
               
                if (tex_x < (unsigned int)game->skybox_tex->width &&
                    tex_y < (unsigned int)game->skybox_tex->height)
                {
                    draw_pixel(game, x, y,
                        get_texture_pixel(game->skybox_tex, tex_x, tex_y));
                }
            }
        }
    }
}
/*
int render_frame(t_game *game)
{

    t_scanline buffer;

    init_scanline_buffer(&buffer);
    
    if (game->skybox_tex)
    {
        draw_skybox(game);
    }
    if (game->bsp_tree && game->bsp_tree->root)
    {
	    render_bsp_node(game, game->bsp_tree->root, &buffer);
    }
    move_player(game);
    draw_weapon(game);
    update_fps(game);
    swap_buffers(game);
    return (0);
}*/