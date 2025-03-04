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



int	load_sound(uint32_t *sound, char *path)
{
	static int	loaded;

	if (!loaded)
		loaded = BASS_Init(-1, 44100, 0, 0, 0);
	if (!loaded || *sound)
		return (0);
	*sound = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);
	if (!*sound)
	{
		ft_printf("Error loading sound: %s (Error code: %d)\n", 
			path, BASS_ErrorGetCode());
		return (0);
	}
	return (1);
}

void	play_sound_loop(uint32_t sound, int play, int loop)
{
	if (!sound)
		return ;
	if (loop)
		BASS_ChannelFlags(sound, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	if (play)
		BASS_ChannelPlay(sound, 0);
	else
		BASS_ChannelPause(sound);
}

void	play_sound(uint32_t sound)
{
	if (!sound)
		return ;
	BASS_ChannelStop(sound);
	BASS_ChannelSetPosition(sound, 0, 0);
	BASS_ChannelPlay(sound, 0);
}

void	play_game_sound(t_game *game, t_sound_type sound_type)
{
	if (!game || !game->sounds || !game->sounds->is_initialized)
		return ;
		
	if (sound_type == SOUND_DOOR)
		play_sound(game->sounds->door);
	else if (sound_type == SOUND_GUN)
		play_sound(game->sounds->gun);
	else if (sound_type == SOUND_FOOTSTEP)
		play_sound(game->sounds->footstep);
	else if (sound_type == SOUND_AMBIENT)
		play_sound_loop(game->sounds->ambient, 1, 1);
	else if (sound_type == SOUND_HURT)
		play_sound(game->sounds->hurt);
	else if (sound_type == SOUND_ENEMY)
		play_sound(game->sounds->enemy);
}

