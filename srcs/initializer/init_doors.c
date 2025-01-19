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


void init_door(t_door *door, int x, int y, t_door_orientation orient)
{
    door->position = (t_vector_i){x, y};
    door->state = DOOR_CLOSED;
    door->orient = orient;
    door->animation = 0.0;
    door->active = true;
    door->timer = 0.0;
    door->locked = false;
    door->key_type = 0;
}

static t_door_orientation    get_door_orientation(t_game *game, int x, int y)
{
    // Verifica paredes verticais (Norte-Sul)
    if (game->map.grid[y][x - 1] == '1' && game->map.grid[y][x + 1] == '1')
        return (DOOR_HORIZONTAL);
    // Verifica paredes horizontais (Leste-Oeste)
    if (game->map.grid[y - 1][x] == '1' && game->map.grid[y + 1][x] == '1')
        return (DOOR_VERTICAL);
    // Caso não encontre paredes válidas
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

void init_door_system(t_game *game)
{
    // Aloca e inicializa a estrutura do sistema de portas
    game->door_system = ft_calloc(1, sizeof(t_door_system));
    if (!game->door_system)
    {
        ft_printf("Error: Failed to allocate door system\n");
        return;
    }

    // Inicializa os valores do sistema
    game->door_system->doors = NULL;
    game->door_system->door_count = 0;

    // Carrega a textura da porta
    t_texture *door_tex = texture_create(game, DOOR1);
    if (!door_tex)
    {
        ft_printf("Error: Failed to load door texture\n");
        free(game->door_system);
        game->door_system = NULL;
        return;
    }
    game->door_system->door_texture = *door_tex;
    free(door_tex);

    // Procura por portas no mapa
    scan_map_for_doors(game);
}

void add_door(t_game *game, int x, int y)
{
    t_door_system *ds = game->door_system;
    t_door *new_doors;
    
    // Aloca novo array de portas com um elemento a mais
    new_doors = ft_calloc(ds->door_count + 1, sizeof(t_door));
    if (!new_doors)
    {
        ft_printf("Error: Failed to allocate memory for new door\n");
        return;
    }

    // Copia portas existentes para o novo array
    if (ds->door_count > 0 && ds->doors)
    {
        ft_memcpy(new_doors, ds->doors, ds->door_count * sizeof(t_door));
        free(ds->doors);
    }

    // Inicializa a nova porta
    ds->doors = new_doors;
    init_door(&ds->doors[ds->door_count], x, y, 
             get_door_orientation(game, x, y));
    ds->door_count++;
}
