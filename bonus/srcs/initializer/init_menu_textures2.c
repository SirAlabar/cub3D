/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_textures2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:55:02 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 18:55:33 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	setup_start_params(t_game *game)
{
	game->menu->start_btn.anim.frame_count = 0;
	game->menu->start_btn.anim.current_frame = 0;
	game->menu->start_btn.anim.frame_duration = 100.0;
	game->menu->start_btn.anim.last_update = get_time_ms();
	game->menu->start_btn.anim.active = 1;
	game->menu->start_btn.selected = 1;
}

int	init_menu_start(t_game *game)
{
	t_texture	*tex;
	int			i;
	char		*paths[6];

	paths[0] = START1;
	paths[1] = START2;
	paths[2] = START3;
	paths[3] = START4;
	paths[4] = START5;
	paths[5] = START6;
	setup_start_params(game);
	i = -1;
	while (++i < 6)
	{
		tex = texture_create(game, paths[i]);
		if (tex)
		{
			game->menu->start_btn.anim.frames[i] = *tex;
			free(tex);
			game->menu->start_btn.anim.frame_count++;
		}
		else
			return (0);
	}
	return (1);
}

static void	setup_exit_params(t_game *game)
{
	game->menu->exit_btn.anim.frame_count = 0;
	game->menu->exit_btn.anim.current_frame = 0;
	game->menu->exit_btn.anim.frame_duration = 100.0;
	game->menu->exit_btn.anim.last_update = get_time_ms();
	game->menu->exit_btn.anim.active = 1;
	game->menu->exit_btn.selected = 0;
}

int	init_menu_exit(t_game *game)
{
	t_texture	*tex;
	int			i;
	char		*paths[6];

	paths[0] = EXIT1;
	paths[1] = EXIT2;
	paths[2] = EXIT3;
	paths[3] = EXIT4;
	paths[4] = EXIT5;
	paths[5] = EXIT6;
	setup_exit_params(game);
	i = -1;
	while (++i < 6)
	{
		tex = texture_create(game, paths[i]);
		if (tex)
		{
			game->menu->exit_btn.anim.frames[i] = *tex;
			free(tex);
			game->menu->exit_btn.anim.frame_count++;
		}
		else
			return (0);
	}
	return (1);
}

int	init_select_gun(t_game *game)
{
	t_texture	*tex;

	game->menu->select_gun.frame_count = 0;
	game->menu->select_gun.current_frame = 0;
	game->menu->select_gun.frame_duration = 0;
	game->menu->select_gun.last_update = get_time_ms();
	game->menu->select_gun.active = 1;
	tex = texture_create(game, SELECT_GUN);
	if (!tex)
		return (0);
	game->menu->select_gun.frames[0] = *tex;
	free(tex);
	game->menu->select_gun.frame_count = 1;
	return (1);
}
