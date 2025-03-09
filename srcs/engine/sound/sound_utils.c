/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourname <yourname@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:00:00 by yourname          #+#    #+#             */
/*   Updated: 2025/03/04 18:00:00 by yourname         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	clear_sounds(t_game *game)
{
	if (!game->sounds)
		return ;
	if (game->sounds->door)
		BASS_StreamFree(game->sounds->door);
	if (game->sounds->gun)
		BASS_StreamFree(game->sounds->gun);
	if (game->sounds->footstep)
		BASS_StreamFree(game->sounds->footstep);
	if (game->sounds->ambient)
		BASS_StreamFree(game->sounds->ambient);
	if (game->sounds->hurt)
		BASS_StreamFree(game->sounds->hurt);
	if (game->sounds->enemy)
		BASS_StreamFree(game->sounds->enemy);
	BASS_Free();
	free(game->sounds);
	game->sounds = NULL;
}

void	init_sound_empty(t_game *game)
{
	if (!game->sounds)
		return ;
	game->sounds->door = 0;
	game->sounds->gun = 0;
	game->sounds->footstep = 0;
	game->sounds->ambient = 0;
	game->sounds->hurt = 0;
	game->sounds->enemy = 0;
}

int	init_sounds(t_game *game)
{
	game->sounds = ft_calloc(1, sizeof(t_sounds));
	if (!game->sounds)
		return (0);
	init_sound_empty(game);
	if (!load_sound(&game->sounds->door, SOUND_FILE_DOOR))
		ft_printf("Warning: Failed to load door sound\n");
	if (!load_sound(&game->sounds->gun, SOUND_FILE_GUN))
		ft_printf("Warning: Failed to load gun sound\n");
	if (!load_sound(&game->sounds->footstep, SOUND_FILE_FOOTSTEP))
		ft_printf("Warning: Failed to load footstep sound\n");
	if (!load_sound(&game->sounds->ambient, SOUND_FILE_AMBIENT))
		ft_printf("Warning: Failed to load ambient sound\n");
	if (!load_sound(&game->sounds->hurt, SOUND_FILE_HURT))
		ft_printf("Warning: Failed to load hurt sound\n");
	if (!load_sound(&game->sounds->enemy, SOUND_FILE_ENEMY))
		ft_printf("Warning: Failed to load enemy sound\n");
	if (game->sounds->ambient)
	{
		play_sound_loop(game->sounds->ambient, 1, 1);
		set_sound_volume(game->sounds->ambient, 0.3);
	}
	game->sounds->is_initialized = true;
	return (1);
}
