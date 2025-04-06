/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_menu_textures.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:37:29 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 18:57:23 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	setup_bg_params(t_game *game)
{
	game->menu->bg.frame_count = 0;
	game->menu->bg.current_frame = 0;
	game->menu->bg.frame_duration = 300.0;
	game->menu->bg.last_update = get_time_ms();
	game->menu->bg.active = 1;
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
			game->menu->bg.frames[i] = *tex;
			free(tex);
			game->menu->bg.frame_count++;
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
	game->menu->cube.frame_duration = 250.0;
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
