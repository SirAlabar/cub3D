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
/*
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
}*/


static void render_leaf_node(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
	int			i;
	t_fixed_vec32 v1;
	t_fixed_vec32 v2;

	i = -1;
	while (++i < node->num_lines)
	{
		if (node->lines[i])
		{
			v1 = transform_point(node->lines[i]->start, game);
			v2 = transform_point(node->lines[i]->end, game);
			if (is_segment_visible(v1, v2))
				render_wall_segment(game, node->lines[i], buffer);
		}
	}
}
void render_bsp_node(t_game *game, t_bsp_node *node, t_scanline *buffer)
{
	t_bsp_side	side;
	t_fixed_vec32 v1;
	t_fixed_vec32 v2;

	if (!node)
		return;

	// Se este é um nó interno com um segmento de partição
	if (node->partition)
	{
		side = bsp_classify_point(game->p1.pos, node->partition);
		
		// Renderiza na ordem correta (de trás para frente)
		if (side == BSP_FRONT)
		{
			// Primeiro renderiza o lado "back"
			render_bsp_node(game, node->back, buffer);
			
			// Depois renderiza apenas o segmento de partição deste nó
			v1 = transform_point(node->partition->start, game);
			v2 = transform_point(node->partition->end, game);
			if (is_segment_visible(v1, v2))
				render_wall_segment(game, node->partition, buffer);
			
			// Por último, renderiza o lado "front"
			render_bsp_node(game, node->front, buffer);
		}
		else // BSP_BACK ou BSP_COLINEAR
		{
			// Primeiro renderiza o lado "front"
			render_bsp_node(game, node->front, buffer);
			
			// Depois renderiza apenas o segmento de partição deste nó
			v1 = transform_point(node->partition->start, game);
			v2 = transform_point(node->partition->end, game);
			if (is_segment_visible(v1, v2))
				render_wall_segment(game, node->partition, buffer);
			
			// Por último, renderiza o lado "back"
			render_bsp_node(game, node->back, buffer);
		}
	}
	else // Nó folha - renderiza todos os segmentos neste nó
	{
		render_leaf_node(game, node, buffer);
	}
}

void draw_skybox(t_game *game)
{
    t_vector_i pos;
    unsigned int view_angle;
	unsigned int tex_x;
	unsigned int tex_y;
	int screen_pos;
	unsigned int angle_offset;
    
    pos.y = -1;
    while (++pos.y < WINDOW_HEIGHT >> 1)
    {
        pos.x = -1;
        while (++pos.x < WINDOW_WIDTH)
        {
            screen_pos = pos.x - (WINDOW_WIDTH >> 1);
            angle_offset = (unsigned int)((long long)screen_pos * (FOV >> 1) / (WINDOW_WIDTH >> 1));
            view_angle = (game->p1.angle + angle_offset) & ANGLEMASK;
			tex_x = ((view_angle * (unsigned long long)game->skybox_tex->width) / ANG360) % game->skybox_tex->width;
            tex_y = (pos.y * game->skybox_tex->height) / (WINDOW_HEIGHT >> 1);
            if (tex_x < (unsigned int)game->skybox_tex->width &&
                tex_y < (unsigned int)game->skybox_tex->height)
            {
                draw_pixel(game, pos.x, pos.y, 
                    get_texture_pixel(game->skybox_tex, tex_x, tex_y));
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