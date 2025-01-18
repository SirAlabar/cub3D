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


void	init_door(t_door *door, int x, int y, t_door_orientation orient)
{
	door->position = (t_vector_i){x, y};
	door->state = DOOR_CLOSED;
	door->orient = orient;
	door->animation = 0.0;
	door->timer = 0.0;
	door->locked = false;
	door->key_type = 0;
}

static t_door_orientation	get_door_orientation(t_game *game, int x, int y)
{
	int	ns_walls;
	int	ew_walls;

	ns_walls = (game->map.grid[y - 1][x] == '1' && game->map.grid[y + 1][x] == '1');
	ew_walls = (game->map.grid[y][x - 1] == '1' && game->map.grid[y][x + 1] == '1');
	if (ns_walls)
		return (DOOR_VERTICAL);
	if (ew_walls)
		return (DOOR_HORIZONTAL);
	return (DOOR_VERTICAL);
}

static void	scan_map_for_doors(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (is_door(game->map.grid[y][x]))
				add_door(game, x, y);
			x++;
		}
		y++;
	}
}

void	init_door_system(t_game *game)
{
	game->door_system = malloc(sizeof(t_door_system));
	if (!game->door_system)
	{
		ft_printf("Error: Failed to allocate door system\n");
		return ;
	}
	game->door_system->doors = NULL;
	game->door_system->door_count = 0;
	game->door_system->door_texture = *texture_create(game, DOOR1);
	if (!game->door_system->door_texture.img)
	{
		ft_printf("Error: Failed to load door texture\n");
		cleanup_door_system(game);
		return ;
	}
	scan_map_for_doors(game);
}

void	add_door(t_game *game, int x, int y)
{
	t_door			*new_doors;
	t_door_system	*ds;
	t_door			*door;

	ds = game->door_system;
	new_doors = ft_calloc((ds->door_count + 1), sizeof(t_door));
	if (!new_doors)
	{
		ft_printf("Error: Failed to allocate new door\n");
		return ;
	}
	ds->doors = new_doors;
	door = &ds->doors[ds->door_count];
	init_door(door, x, y, get_door_orientation(game, x, y));
	ds->door_count++;
}
