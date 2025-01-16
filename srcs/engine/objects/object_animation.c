/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/15 20:27:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	render_sprite(t_game *game, t_sprite *sprite)
{
	double	transform_x;
	double	transform_y;
	int		x;

	transform_sprite(game, sprite, &transform_x, &transform_y);
	if (transform_y <= 0)
		return ;
	calculate_dimensions(game, sprite, transform_x, transform_y);
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		if (x >= sprite->tex.x - sprite->tex.y / 2
			&& x <= sprite->tex.x + sprite->tex.y / 2
			&& transform_y < game->rays[x].perp_wall_dist)
			render_sprite_line(game, sprite, x, transform_y);
		x++;
	}
}

void	update_objects_animations(t_game *game, double delta_time)
{
	t_sprite	*sprite;
	int			i;

	i = 0;
	while (i < game->num_sprites)
	{
		sprite = &game->sprites[i];
		if (sprite->is_animated)
		{
			sprite->anim_time += delta_time;
			if (sprite->anim_time >= 0.1)
			{
				sprite->current_frame = (sprite->current_frame + 1) % 4;
				sprite->anim_time = 0;
			}
		}
		i++;
	}
}

void	render_sprites(t_game *game)
{
	int		i;

	update_objects_animations(game, get_delta_time());
	i = 0;
	while (i < game->num_sprites)
	{
		render_sprite(game, &game->sprites[i]);
		i++;
	}
}
