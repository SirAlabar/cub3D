/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_doors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:49:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_door_orientation	get_door_orientation(t_game *game, int x, int y)
{
	if (game->map.grid[y][x - 1] == '1' && game->map.grid[y][x + 1] == '1')
		return (DOOR_HORIZONTAL);
	if (game->map.grid[y - 1][x] == '1' && game->map.grid[y + 1][x] == '1')
		return (DOOR_VERTICAL);
	return (cleanup_game(game), DOOR_ERROR);
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
			{
				add_door(game, x, y);
			}
			x++;
		}
		y++;
	}
}

static char	*get_random_door(void)
{
	int	random_num;

	random_num = rand() % 3 + 1;
	if (random_num == 1)
		return ((DOOR1));
	if (random_num == 2)
		return ((DOOR2));
	return ((DOOR3));
}

void	init_door_system(t_game *game)
{
	t_texture	*door_tex;
	char		*door_path;

	game->door_system = ft_calloc(1, sizeof(t_door_system));
	if (!game->door_system)
	{
		ft_printf("Error: Failed to allocate door system\n");
		return ;
	}
	game->door_system->doors = NULL;
	game->door_system->door_count = 0;
	door_path = get_random_door();
	door_tex = texture_create(game, door_path);
	if (!door_tex)
	{
		ft_printf("Error: Failed to load door texture\n");
		free(game->door_system);
		game->door_system = NULL;
		return ;
	}
	game->door_system->door_texture = *door_tex;
	free(door_tex);
	scan_map_for_doors(game);
}

void	add_door(t_game *game, int x, int y)
{
	t_door_system	*ds;
	t_door			*new_doors;

	ds = game->door_system;
	new_doors = ft_calloc(ds->door_count + 1, sizeof(t_door));
	if (!new_doors)
	{
		ft_printf("Error: Failed to allocate memory for new door\n");
		return ;
	}
	if (ds->door_count > 0 && ds->doors)
	{
		ft_memcpy(new_doors, ds->doors, ds->door_count * sizeof(t_door));
		free(ds->doors);
	}
	ds->doors = new_doors;
	init_door(&ds->doors[ds->door_count], x, y, get_door_orientation(game, x,
			y));
	ds->door_count++;
}
