/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 16:54:35 by hluiz-ma         ###   ########.fr       */
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

	north = texture_create(game, "./assets/texture/metal.xpm");
	south = texture_create(game, "./assets/texture/metal2.xpm");
	east = texture_create(game, "./assets/texture/metal3.xpm");
	west = texture_create(game, "./assets/texture/metal4.xpm");
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

void	init_game(t_game *game)
{
	init_map(game);
	init_player(game);
	init_textures(game);
	//init_textures_game(game);
	game->last_mouse = vector_create(-1, -1);
    game->mouse_sensi = 0.003;
    mlx_mouse_hide(game->mlx, game->win);
}
