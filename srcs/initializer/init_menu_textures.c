/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_textures.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seu-nome <seu-email@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by seu-nome          #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by seu-nome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>


static void	setup_bg_params(t_game *game)
{
	game->menu->background.frame_count = 0;
	game->menu->background.current_frame = 0;
	game->menu->background.frame_duration = 200.0;
	game->menu->background.last_update = get_time_ms();
	game->menu->background.active = 1;
}

int	init_menu_background(t_game *game)
{
	t_texture	*tex;
	int			i;
	char		*paths[4];

	paths[0] = BG1;
	paths[1] = BG2;
	paths[2] = BG3;
	paths[3] = BG4;
	setup_bg_params(game);
	i = -1;
	while (++i < 4)
	{
		tex = texture_create(game, paths[i]);
		if (tex)
		{
			game->menu->background.frames[i] = *tex;
			free(tex);
			game->menu->background.frame_count++;
		}
		else
			return (0);
	}
	return (1);
}

static void	setup_cube_params(t_game *game)
{
	game->menu->cube.frame_count = 0;
	game->menu->cube.current_frame = 0;
	game->menu->cube.frame_duration = 150.0;
	game->menu->cube.last_update = get_time_ms();
	game->menu->cube.active = 1;
}

int	init_menu_cube(t_game *game)
{
	t_texture	*tex;
	int			i;
	char		*paths[5];

	paths[0] = CUBE1;
	paths[1] = CUBE2;
	paths[2] = CUBE3;
	paths[3] = CUBE4;
	paths[4] = CUBE5;
	setup_cube_params(game);
	i = -1;
	while (++i < 5)
	{
		tex = texture_create(game, paths[i]);
		if (tex)
		{
			game->menu->cube.frames[i] = *tex;
			free(tex);
			game->menu->cube.frame_count++;
		}
		else
			return (0);
	}
	return (1);
}

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