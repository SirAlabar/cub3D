/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_sounds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:02:57 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:49:08 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_sound_resource(uint32_t *sound)
{
	if (!sound || !*sound)
		return ;
	BASS_ChannelStop(*sound);
	BASS_StreamFree(*sound);
	*sound = 0;
}

void	cleanup_sounds(t_game *game)
{
	if (!game || !game->sounds)
		return ;
	if (game->sounds->door)
		free_sound_resource(&game->sounds->door);
	if (game->sounds->gun)
		free_sound_resource(&game->sounds->gun);
	if (game->sounds->footstep)
		free_sound_resource(&game->sounds->footstep);
	if (game->sounds->ambient)
		free_sound_resource(&game->sounds->ambient);
	if (game->sounds->hurt)
		free_sound_resource(&game->sounds->hurt);
	if (game->sounds->enemy)
		free_sound_resource(&game->sounds->enemy);
	BASS_Free();
	game->sounds->is_initialized = false;
	free(game->sounds);
	game->sounds = NULL;
}
