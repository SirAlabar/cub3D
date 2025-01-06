/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/06 20:16:41 by hluiz-ma         ###   ########.fr       */
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
        "10100001",
        "10000101",
        "10000001",
        "10100001",
        "10000001",
        "11111111"
    };

    // Copy map data
    for (i = 0; i < map_height; i++)
        ft_strlcpy(game->map.grid[i], map_data[i], map_width + 1);

    // Set player initial position (center of the map)
    game->p1.pos = vector_create(3.5, 3.5);
    game->p1.dir = vector_create(-1, 0);  // Facing west
 	//game->p1.dir = vector_create(-0.7071, -0.7071);
	//game->p1.dir = vector_create(0.3371, 0.3371);	
    game->p1.plane = vector_create(0, tan(FOV * M_PI / 360.0));
}

void move_player(t_game *game, double dir_x, double dir_y)
{
    double new_x = game->p1.pos.x + dir_x * game->p1.move_speed;
    double new_y = game->p1.pos.y + dir_y * game->p1.move_speed;
    
    // Verificar colisão antes de mover
    if (game->map.grid[(int)new_x][(int)game->p1.pos.y] != '1')
        game->p1.pos.x = new_x;
    if (game->map.grid[(int)game->p1.pos.x][(int)new_y] != '1')
        game->p1.pos.y = new_y;
}

void rotate_player(t_game *game, double angle)
{
    double old_dir_x = game->p1.dir.x;
    double old_plane_x = game->p1.plane.x;
    
    // Rotacionar direção
    game->p1.dir.x = game->p1.dir.x * cos(angle) - game->p1.dir.y * sin(angle);
    game->p1.dir.y = old_dir_x * sin(angle) + game->p1.dir.y * cos(angle);
    
    // Rotacionar plano da câmera
    game->p1.plane.x = game->p1.plane.x * cos(angle) - game->p1.plane.y * sin(angle);
    game->p1.plane.y = old_plane_x * sin(angle) + game->p1.plane.y * cos(angle);
}

int key_handler(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
        close_window(game);
    else if (keycode == KEY_W)
        move_player(game, game->p1.dir.x, game->p1.dir.y);
    else if (keycode == KEY_S)
        move_player(game, -game->p1.dir.x, -game->p1.dir.y);
    else if (keycode == KEY_D)
        move_player(game, game->p1.plane.x, game->p1.plane.y);
    else if (keycode == KEY_A)
        move_player(game, -game->p1.plane.x, -game->p1.plane.y);
    else if (keycode == KEY_LEFT)
        rotate_player(game, game->p1.rot_speed);
    else if (keycode == KEY_RIGHT)
        rotate_player(game, -game->p1.rot_speed);

    engine_render_frame(game);
    return (0);
}