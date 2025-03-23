/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:38:13 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 18:38:13 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	gun_coordinates(int position, t_vector *pos, t_texture *texture)
{
	pos->x = WINDOW_WIDTH / 6;
	if (position == 1)
		pos->y = WINDOW_HEIGHT * 0.6;
	else
		pos->y = WINDOW_HEIGHT * 0.75;
	pos->y -= texture->height >> 1;
}

void	draw_select_gun(t_game *game, int position)
{
	t_texture		*gun_texture;
	t_vector		pos;
	int				y;
	int				x;
	unsigned int	color;

	gun_texture = &game->menu->select_gun.frames[0];
	if (!gun_texture->img)
		return ;
	gun_coordinates(position, &pos, gun_texture);
	y = -1;
	while (++y < gun_texture->height)
	{
		x = -1;
		while (++x < gun_texture->width)
		{
			if (pos.x + x < 0 || pos.x + x >= WINDOW_WIDTH || pos.y + y < 0
				|| pos.y + y >= WINDOW_HEIGHT)
				continue ;
			color = get_texture_pixel(gun_texture, x, y);
			if ((color & 0xFFC0CB) != 0)
				draw_pixel(game, pos.x + x, pos.y + y, color);
		}
	}
}

static void	process_animation_pixels(t_game *game, t_texture *frame,
		t_vector pos)
{
	t_vector_i	pixel;
	int			color;

	pixel.y = -1;
	while (++pixel.y < frame->height)
	{
		pixel.x = -1;
		while (++pixel.x < frame->width)
		{
			if (pos.x + pixel.x < 0 || pos.x + pixel.x >= WINDOW_WIDTH || pos.y
				+ pixel.y < 0 || pos.y + pixel.y >= WINDOW_HEIGHT)
				continue ;
			color = get_texture_pixel(frame, pixel.x, pixel.y);
			if ((color & 0xFFC0CB) != 0)
				draw_pixel(game, pos.x + pixel.x, pos.y + pixel.y, color);
		}
	}
}

void	draw_animation(t_game *game, t_animation *anim)
{
	t_texture	*current_frame;
	t_vector	pos;

	if (!anim)
		return ;
	if (!anim->active || anim->frame_count == 0)
		return ;
	current_frame = &anim->frames[anim->current_frame];
	if (!current_frame->img)
		return ;
	pos.x = (WINDOW_WIDTH - current_frame->width) >> 1;
	pos.y = (WINDOW_HEIGHT - current_frame->height) >> 1;
	process_animation_pixels(game, current_frame, pos);
}

void	draw_menu(t_game *game)
{
	if (!game || !game->menu)
		return ;
	update_menu_animations(game);
	if (game->menu->bg.active)
		draw_animation(game, &game->menu->bg);
	if (game->menu->cube.active)
		draw_animation(game, &game->menu->cube);
	if (game->menu->start_btn.anim.active)
		draw_animation(game, &game->menu->start_btn.anim);
	if (game->menu->exit_btn.anim.active)
		draw_animation(game, &game->menu->exit_btn.anim);
	if (game->menu->selected_option == 0)
		draw_select_gun(game, 1);
	else
		draw_select_gun(game, 2);
}
