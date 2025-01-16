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

static bool	try_place_object(t_game *game, t_object_type type)
{
	t_vector		pos;
	t_object_config	config;
	t_placement_type	placement;

	init_object_config(&config, type);
	placement = config.placement;
	pos = get_random_position(game, placement);
	if (pos.x == -1)
		return (false);

	if (game->map.num_sprites >= game->map.max_sprites)
		return (false);

	return (init_sprite(game, &game->map.sprites[game->map.num_sprites++],
			pos, type));
}
