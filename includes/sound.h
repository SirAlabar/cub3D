/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sound.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:00:00 by yourname          #+#    #+#             */
/*   Updated: 2025/03/24 19:46:34 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOUND_H
# define SOUND_H

# include "../libs/42-Bass/bass.h"
# include <stdbool.h>
# include <stdint.h>

typedef enum e_sound_type
{
	SOUND_DOOR,
	SOUND_GUN,
	SOUND_PORTAL,
	SOUND_FOOTSTEP,
	SOUND_AMBIENT,
	SOUND_HURT,
	SOUND_ENEMY,
	SOUND_COUNT
}						t_sound_type;

# define SOUND_FILE_DOOR "./assets/sound/door.wav"
# define SOUND_FILE_GUN "./assets/sound/gun_shot.wav"
# define SOUND_FILE_PORTAL "./assets/sound/portal_effect.wav"
# define SOUND_FILE_FOOTSTEP "./assets/sound/footstep.wav"
# define SOUND_FILE_AMBIENT "./assets/sound/ambient2.wav"
# define SOUND_FILE_HURT "./assets/sound/hurt.wav"
# define SOUND_FILE_ENEMY "./assets/sound/enemy.wav"

typedef struct s_sounds
{
	uint32_t			door;
	uint32_t			gun;
	uint32_t			portal;
	uint32_t			footstep;
	uint32_t			ambient;
	uint32_t			hurt;
	uint32_t			enemy;
	bool				is_initialized;
}						t_sounds;

typedef struct s_game	t_game;

void					cleanup_sounds(t_game *game);
void					init_sound_empty(t_game *game);
int						init_sounds(t_game *game);

int						load_sound(uint32_t *sound, char *path);
void					play_sound_loop(uint32_t sound, int play, int loop);
void					play_sound(uint32_t sound);
void					play_game_sound(t_game *game, t_sound_type sound_type);
void					set_sound_volume(uint32_t sound, float volume);

#endif