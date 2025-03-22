/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:33:10 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void draw_select_gun(t_game *game, int position)
{
    t_texture   *gun_texture;
    t_vector    pos;
    int         y;
    int         x;
    unsigned int color;

    gun_texture = &game->menu->select_gun.frames[0];
    if (!gun_texture->img)
        return ;
    pos.x = WINDOW_WIDTH / 6;
    if (position == 1)
        pos.y = WINDOW_HEIGHT * 0.7;
    else
        pos.y = WINDOW_HEIGHT * 0.85;
    
    pos.y -= gun_texture->height / 2;
    y = -1;
    while (++y < gun_texture->height)
    {
        x = -1;
        while (++x < gun_texture->width)
        {
            if (pos.x + x < 0 || pos.x + x >= WINDOW_WIDTH
                || pos.y + y < 0 || pos.y + y >= WINDOW_HEIGHT)
                continue ;
            
            color = get_texture_pixel(gun_texture, x, y);
            if ((color & 0x00FFFFFF) != 0)
                draw_pixel(game, pos.x + x, pos.y + y, color);
        }
    }
}

void draw_animation(t_game *game, t_animation *anim)
{
    t_texture   *current_frame;
    int         y;
    int         x;
    int         pos_x;
    int         pos_y;
    unsigned int color;

    if (!anim->active || anim->frame_count == 0)
        return ;
    current_frame = &anim->frames[anim->current_frame];
    if (!current_frame->img)
        return ;
    pos_x = (WINDOW_WIDTH - current_frame->width) / 2;
    pos_y = (WINDOW_HEIGHT - current_frame->height) / 2;
    y = -1;
    while (++y < current_frame->height)
    {
        x = -1;
        while (++x < current_frame->width)
        {
            if (pos_x + x < 0 || pos_x + x >= WINDOW_WIDTH
                || pos_y + y < 0 || pos_y + y >= WINDOW_HEIGHT)
                continue ;
            color = get_texture_pixel(current_frame, x, y);
            if ((color & 0x00FFFFFF) != 0)
                draw_pixel(game, pos_x + x, pos_y + y, color);
        }
    }
}

void	draw_menu(t_game *game)
{
	update_menu_animations(game);
	draw_animation(game, &game->menu->background);
	draw_animation(game, &game->menu->cube);
	draw_animation(game, &game->menu->start_btn.anim);
	draw_animation(game, &game->menu->exit_btn.anim);
	if (game->menu->selected_option == 0)
		draw_select_gun(game, 1);
	else
		draw_select_gun(game, 2);
}
