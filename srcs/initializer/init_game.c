/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/10 21:08:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_game(t_game *game)
{
	t_texture	*north;
	t_texture	*south;
	t_texture	*east;
	t_texture	*west;

	init_map(game);
	// init_textures(game);
	init_player(game);
	// test
	init_map(game);
	init_player(game);
	north = texture_create(game, "./texture/north.xpm");
	south = texture_create(game, "./texture/south.xpm");
	east = texture_create(game, "./texture/east.xpm");
	west = texture_create(game, "./texture/west.xpm");
	if (!north || !south || !east || !west)
	{
		ft_printf("Error\nFailed to load textures\n");
		if (north)
			free(north);
		if (south)
			free(south);
		if (east)
			free(east);
		if (west)
			free(west);
		exit(1);
	}
	game->north = *north;
	game->south = *south;
	game->east = *east;
	game->west = *west;
	free(north);
	free(south);
	free(east);
	free(west);
	// test
}
