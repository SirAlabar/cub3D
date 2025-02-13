/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/13 18:06:00 by hluiz-ma         ###   ########.fr       */
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

    if (node->partition)
        side = bsp_classify_point(game->p1.pos, node->partition);
    else
        side = BSP_FRONT;

    if (side == BSP_FRONT || side == BSP_COLINEAR)
    {
        if (node->back)
            render_bsp_node(game, node->back, buffer);

        for (i = 0; i < node->num_lines; i++)
        {
            if (node->lines[i])
                render_wall_segment(game, node->lines[i], buffer);
        }

        if (node->front)
            render_bsp_node(game, node->front, buffer);
    }
    else
    {
        if (node->front)
            render_bsp_node(game, node->front, buffer);

        for (i = 0; i < node->num_lines; i++)
        {
            if (node->lines[i])
                render_wall_segment(game, node->lines[i], buffer);
        }

        if (node->back)
            render_bsp_node(game, node->back, buffer);
    }
}

int render_frame(t_game *game)
{
    t_scanline buffer;
    
    init_scanline_buffer(&buffer);
    draw_background(game);

    if (game->bsp_tree && game->bsp_tree->root)
    {
        ft_printf("Player position: (%d,%d)\n", 
            fixed32_to_int(game->p1.pos.x),
            fixed32_to_int(game->p1.pos.y));
        ft_printf("Player angle: %d\n", 
            fixed32_to_int(game->p1.angle));
            
        render_bsp_node(game, game->bsp_tree->root, &buffer);
    }

    draw_weapon(game);
    update_fps(game);
    swap_buffers(game);
    return (0);
}