/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 22:00:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_vector_i	get_weapon_dimensions(t_texture *gun)
{
	t_vector_i	dim;

	dim.x = WINDOW_WIDTH / 4;
	dim.y = (dim.x * gun->height) / gun->width;
	return (dim);
}

static t_vector_i	get_weapon_position(t_vector_i dim)
{
	t_vector_i	pos;

	pos.x = (WINDOW_WIDTH - dim.x) / 2;
	pos.y = WINDOW_HEIGHT - dim.y;
	return (pos);
}

static t_vector	get_scale_factors(t_texture *gun, t_vector_i dim)
{
	t_vector	scale;

	scale.x = (double)gun->width / dim.x;
	scale.y = (double)gun->height / dim.y;
	return (scale);
}

static void	draw_weapon_pixel(t_game *game, t_vector_i pos, t_vector_i gun_pos,
		t_texture *gun)
{
	t_vector	src;
	t_vector	scale;
	int			color;

	scale = get_scale_factors(gun, get_weapon_dimensions(gun));
	src.x = (pos.x - gun_pos.x) * scale.x;
	src.y = (pos.y - gun_pos.y) * scale.y;
	if ((int)src.x >= gun->width || (int)src.y >= gun->height)
		return ;
	color = get_texture_pixel(gun, (int)src.x, (int)src.y);
	if (color != 0xFFC0CB)
		draw_pixel(game, pos.x, pos.y, color);
}

void	draw_weapon(t_game *game)
{
	t_texture	*current_gun;
	t_vector_i	dim;
	t_vector_i	gun_pos;
	t_vector_i	pos;

	current_gun = &game->p1.gun_anim[game->p1.current_frame];
	if (!current_gun || !current_gun->img)
		return ;
	dim = get_weapon_dimensions(current_gun);
	gun_pos = get_weapon_position(dim);
	pos.y = gun_pos.y - 1;
	while (++pos.y < gun_pos.y + dim.y)
	{
		pos.x = gun_pos.x - 1;
		while (++pos.x < gun_pos.x + dim.x)
			draw_weapon_pixel(game, pos, gun_pos, current_gun);
	}
}
