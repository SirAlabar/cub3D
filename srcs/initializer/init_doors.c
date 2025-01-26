/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_doors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/26 15:54:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_door(t_door *door, int x, int y, t_door_orientation orient)
{
	door->position = (t_vector_i){y, x};
	door->state = DOOR_CLOSED;
	door->orient = orient;
	door->animation = 0.0;
	door->active = true;
	door->timer = 0.0;
	door->locked = false;
	door->key_type = 0;
}
/*
static t_door_orientation	get_door_orientation(t_game *game, int x, int y)
{
	if (game->map.grid[y][x - 1] == '1' && game->map.grid[y][x + 1] == '1')
		return (DOOR_HORIZONTAL);
	if (game->map.grid[y - 1][x] == '1' && game->map.grid[y + 1][x] == '1')
		return (DOOR_VERTICAL);
	return (DOOR_VERTICAL);
}*/

static t_door_orientation get_door_orientation(t_game *game, int x, int y)
{
    printf("\nChecking door orientation at [%d,%d]:\n", x, y);
    
    // Primeiro verifica se há portas adjacentes
    int has_vertical_doors = 0;
    if (y > 0 && game->map.grid[y - 1][x] == 'D')
        has_vertical_doors = 1;
    if (y < game->map.height - 1 && game->map.grid[y + 1][x] == 'D')
        has_vertical_doors = 1;
        
    int has_horizontal_doors = 0;
    if (x > 0 && game->map.grid[y][x - 1] == 'D')
        has_horizontal_doors = 1;
    if (x < game->map.width - 1 && game->map.grid[y][x + 1] == 'D')
        has_horizontal_doors = 1;
    
    // Se tem portas adjacentes verticais, prioriza essa orientação
    if (has_vertical_doors)
    {
        printf("Door is VERTICAL (connected to other doors vertically)\n");
        return (DOOR_VERTICAL);
    }
    
    // Se tem portas adjacentes horizontais, prioriza essa orientação
    if (has_horizontal_doors)
    {
        printf("Door is HORIZONTAL (connected to other doors horizontally)\n");
        return (DOOR_HORIZONTAL);
    }
    
    // Se não tem portas adjacentes, verifica as paredes
    if (game->map.grid[y][x - 1] == '1' && game->map.grid[y][x + 1] == '1')
    {
        printf("Door is HORIZONTAL (walls on left and right)\n");
        return (DOOR_HORIZONTAL);
    }
    if (game->map.grid[y - 1][x] == '1' && game->map.grid[y + 1][x] == '1')
    {
        printf("Door is VERTICAL (walls on top and bottom)\n");
        return (DOOR_VERTICAL);
    }
    
    printf("Warning: No clear orientation detected, defaulting to VERTICAL\n");
    return (DOOR_VERTICAL);
}

static void	scan_map_for_doors(t_game *game)
{
	int	y;
	int	x;

	printf("\nScanning map for doors...\n");
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (is_door(game->map.grid[y][x]))
			{
				printf("Found door at [%d,%d]\n", x, y);
				add_door(game, x, y);
			}
			x++;
		}
		y++;
	}
	printf("Total doors found: %d\n", game->door_system->door_count);
}

void	init_door_system(t_game *game)
{
	t_texture	*door_tex;
	t_texture	*doorwall_tex;

	game->door_system = ft_calloc(1, sizeof(t_door_system));
	if (!game->door_system)
	{
		ft_printf("Error: Failed to allocate door system\n");
		return ;
	}
	game->door_system->doors = NULL;
	game->door_system->door_count = 0;
	door_tex = texture_create(game, DOOR1);
	doorwall_tex = texture_create(game, DOOR2);
	if (!door_tex)
	{
		ft_printf("Error: Failed to load door texture\n");
		free(game->door_system);
		game->door_system = NULL;
		return ;
	}
	game->door_system->door_texture = *door_tex;
	game->door_system->doorwall_texture = *doorwall_tex;
	free(door_tex);
	free(doorwall_tex);
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
