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
            *(unsigned int *)(game->addr[game->current_buffer] + pixel_pos) = 0x000000;
            x++;
        }
        y++;
    }
}

void render_bsp_node(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
    t_bsp_side side;
    int i;

    if (!node)
        return;

    // Se não é um nó folha, usa a partição para determinar a ordem
    if (node->partition)
    {
        side = bsp_classify_point(game->p1.pos, node->partition);
        
        // Renderiza de trás pra frente
        if (side == BSP_FRONT)
        {
            render_bsp_node(game, node->back, buffer);
            
            // Renderiza linhas do nó atual
            for (i = 0; i < node->num_lines; i++)
                if (node->lines[i])
                    render_wall_segment(game, node->lines[i], buffer);
            
            render_bsp_node(game, node->front, buffer);
        }
        else // BSP_BACK ou BSP_COLINEAR
        {
            render_bsp_node(game, node->front, buffer);
            
            // Renderiza linhas do nó atual
            for (i = 0; i < node->num_lines; i++)
                if (node->lines[i])
                    render_wall_segment(game, node->lines[i], buffer);
            
            render_bsp_node(game, node->back, buffer);
        }
    }
    // Nó folha - apenas renderiza suas linhas
    else
    {
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

    // Usa 1024 como base, que é a largura da textura
    pixel_per_degree = 1024.0f / 360.0f;

    // Ângulo atual do jogador
    angle = game->p1.angle & ANGLEMASK;
    
    // Conversão de ângulo para graus
    current_angle = ((unsigned int)angle * 360) >> FIXED_POINT_BITS;

    ft_printf("\n=== SKYBOX DEBUG START ===\n");
    ft_printf("Raw Player Angle: %u\n", angle);
    ft_printf("Current Angle: %u\n", current_angle);

    y = -1;
    while (++y < WINDOW_HEIGHT)
    {
        x = -1;
        while (++x < WINDOW_WIDTH)
        {
            // Calcula posição horizontal na textura considerando FOV
            float screen_x_ratio = (float)x / WINDOW_WIDTH;
            
            // Limita o ângulo de acordo com o FOV
            float angle_offset = (screen_x_ratio - 0.5f) * FOV;
            
            // Só renderiza se estiver dentro do FOV
            if (fabs(angle_offset) <= FOV / 2)
            {
                float tex_x_float = fmodf(
                    current_angle + angle_offset, 
                    360.0f
                ) * pixel_per_degree;

                unsigned int tex_x = (unsigned int)tex_x_float;
                unsigned int tex_y = (y * game->skybox_tex->height) / WINDOW_HEIGHT;

                // Desenha pixel
                if (tex_x < (unsigned int)game->skybox_tex->width &&
                    tex_y < (unsigned int)game->skybox_tex->height)
                {
                    draw_pixel(game, x, y,
                        get_texture_pixel(game->skybox_tex, tex_x, tex_y));
                }
            }
        }
    }

    ft_printf("=== SKYBOX DEBUG END ===\n");
}

int render_frame(t_game *game)
{

    t_scanline buffer;

    init_scanline_buffer(&buffer);
    
    if (game->skybox_tex)
    {
        draw_skybox(game);
    }
    else
    {
        draw_background(game);
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
}