/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 22:08:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	free_texture_ptrs(t_texture *n, t_texture *s, t_texture *e,
		t_texture *w)
{
	if (n)
		free(n);
	if (s)
		free(s);
	if (e)
		free(e);
	if (w)
		free(w);
}

void	init_textures_game(t_game *game)
{
	t_texture	*north;
	t_texture	*south;
	t_texture	*east;
	t_texture	*west;

	north = texture_create(game, game->north.path);
	free(game->north.path);
	south = texture_create(game, game->south.path);
	free(game->south.path);
	east = texture_create(game, game->east.path);
	free(game->east.path);
	west = texture_create(game, game->west.path);
	free(game->west.path);
	if (!north || !south || !east || !west)
	{
		ft_printf("Error\nFailed to load textures\n");
		free_texture_ptrs(north, south, east, west);
		cleanup_game(game);
		exit(1);
	}
	game->north = *north;
	game->south = *south;
	game->east = *east;
	game->west = *west;
	free_texture_ptrs(north, south, east, west);
}

void init_shot(t_game *game)
{
	game->shot.active = false;
	game->shot.impact.active = false;
	game->shot.impact.frame = 0;
	game->shot.impact.is_enemy 0 false;
}

void	init_game(t_game *game)
{
	init_map(game);
	if (!is_map_valid(game))
		(printf(RED"Error\ninvalid map\n"DEFAULT));
	init_player(game);
	init_textures(game);
	init_textures_game(game);
	init_shot(game);
	game->last_mouse = vector_create(-1.0, -1.0);
	game->mouse_sensi = 0.003;
	game->fps = 0;
}
