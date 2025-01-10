/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:03:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 18:48:26 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void    init_game(t_game *game)
{
	init_map(game);
	//init_textures(game);
	init_player(game);

//test
	t_texture *north;
    t_texture *south;
    t_texture *east;
    t_texture *west;
    init_map(game);
    init_player(game);
    
    north = texture_create(game, "./texture/north.xpm");
    south = texture_create(game, "./texture/south.xpm");
    east = texture_create(game, "./texture/east.xpm");
    west = texture_create(game, "./texture/west.xpm");

    if (!north || !south || !east || !west)
    {
        ft_printf("Error\nFailed to load textures\n");
        if (north) free(north);
        if (south) free(south);
        if (east) free(east);
        if (west) free(west);
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
//test
}


void	move_player(t_game *game, double dir_x, double dir_y)
{
	double	new_x;
	double	new_y;

	new_x = game->p1.pos.x + dir_x * game->p1.move_speed;
	new_y = game->p1.pos.y + dir_y * game->p1.move_speed;
	if (game->map.grid[(int)new_x][(int)game->p1.pos.y] != '1')
		game->p1.pos.x = new_x;
	if (game->map.grid[(int)game->p1.pos.x][(int)new_y] != '1')
		game->p1.pos.y = new_y;
}

void	rotate_player(t_game *g, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = g->p1.dir.x;
	old_plane_x = g->p1.plane.x;
	g->p1.dir.x = g->p1.dir.x * cos(angle) - g->p1.dir.y * sin(angle);
	g->p1.dir.y = old_dir_x * sin(angle) + g->p1.dir.y * cos(angle);
	g->p1.plane.x = g->p1.plane.x * cos(angle) - g->p1.plane.y * sin(angle);
	g->p1.plane.y = old_plane_x * sin(angle) + g->p1.plane.y * cos(angle);
}

int	key_handler(int keycode, t_game *game)
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
