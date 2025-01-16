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

static t_texture	*load_sprite_texture(t_game *game, char *path)
{
	t_texture	*tex;

	if (!path)
		return (NULL);
	tex = texture_create(game, path);
	if (!tex)
	{
		ft_printf("Error: Failed to load sprite texture: %s\n", path);
		return (NULL);
	}
	return (tex);
}

static bool	load_sprite_animations(t_game *game, t_sprite *sprite,
		t_object_config *config)
{
	int	i;

	if (!config->is_animated)
	{
		sprite->frames[0] = load_sprite_texture(game, config->texture_path);
		if (!sprite->frames[0])
			return (false);
		return (true);
	}
	i = 0;
	while (i < config->num_frames && i < MAX_FRAMES)
	{
		sprite->frames[i] = load_sprite_texture(game, config->frame_paths[i]);
		if (!sprite->frames[i])
			return (false);
		i++;
	}
	return (true);
}

bool	init_sprite(t_game *game, t_sprite *sprite, t_vector pos,
		t_object_type type)
{
	t_object_config	config;

	init_object_config(&config, type);
	sprite->pos = pos;
	sprite->type = type;
	sprite->is_solid = config.is_solid;
	sprite->is_animated = config.is_animated;
	sprite->current_frame = 0;
	sprite->anim_time = 0;
	sprite->distance = 0;
	if (!load_sprite_animations(game, sprite, &config))
	{
		destroy_sprite(game, sprite);
		return (false);
	}
	return (true);
}
