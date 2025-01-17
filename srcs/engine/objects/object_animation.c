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

void	render_sprite(t_game *game, t_sprite *sprite, t_ray *rays)
{
	double	transform_x;
	double	transform_y;
	int		x;

	transform_sprite(game, sprite, &transform_x, &transform_y);
	if (transform_y <= 0)
		return ;
	calculate_dimensions(sprite, transform_x, transform_y);
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		if (x >= sprite->tex.x - sprite->tex.y / 2
			&& x <= sprite->tex.x + sprite->tex.y / 2
			&& transform_y < rays[x].perp_wall_dist)
		{
			sprite->tex.x = x;
			draw_object(game, sprite, transform_y);
		}
		x++;
	}
}

void	update_objects_animations(t_game *game, double delta_time)
{
	t_sprite	*sprite;
	int			i;

	i = 0;
	while (i < game->objects.num_sprites)
	{
		sprite = &game->objects.sprites[i];
		if (sprite->is_animated)
		{
			sprite->anim_time += delta_time;
			if (sprite->anim_time >= 0.1)
			{
				sprite->current_frame = (sprite->current_frame + 1) % MAX_FRAMES;
				sprite->anim_time = 0;
			}
		}
		i++;
	}
}

void	render_sprites(t_game *game, t_ray *rays)
{
	int		i;

	update_objects_animations(game, get_delta_time());
	i = 0;
	while (i < game->objects.num_sprites)
	{
		render_sprite(game, &game->objects.sprites[i], rays);
		i++;
	}
}
