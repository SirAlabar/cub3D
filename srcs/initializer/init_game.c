/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/06 18:43:42 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_game(t_game *game)
{
	ft_memset(game, 0, sizeof(t_game));
	game->map.floor_color = 0x404040;
	game->map.ceiling_color = 0xA0A0A0;
	init_player(game);
}
void init_test_map(t_game *game)
{
    const int map_width = 8;
    const int map_height = 8;
    int i;

    game->map.width = map_width;
    game->map.height = map_height;
    game->map.grid = (char **)malloc(sizeof(char *) * (map_height + 1));
    if (!game->map.grid)
        return;
    game->map.grid[map_height] = NULL;

    // Initialize each row
    for (i = 0; i < map_height; i++)
    {
        game->map.grid[i] = (char *)malloc(sizeof(char) * (map_width + 1));
        if (!game->map.grid[i])
            return;
        game->map.grid[i][map_width] = '\0';
    }

    // Create a simple test map (1 = wall, 0 = empty space)
    const char *map_data[] = {
        "11111111",
        "10000001",
        "10000001",
        "10000001",
        "10000001",
        "10000001",
        "10000001",
        "11111111"
    };

    // Copy map data
    for (i = 0; i < map_height; i++)
        ft_strlcpy(game->map.grid[i], map_data[i], map_width + 1);

    // Set player initial position (center of the map)
    game->p1.pos = vector_create(3.5, 3.5);
    //game->p1.dir = vector_create(-1, 0);  // Facing west
 	game->p1.dir = vector_create(-0.7071, -0.7071);
	//game->p1.dir = vector_create(0.3371, 0.3371);	
    game->p1.plane = vector_create(0, 0.66);
}