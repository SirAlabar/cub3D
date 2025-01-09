/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 20:13:54 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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

void	handle_movement(t_game *game)
{
	double	dir_x;
	double	dir_y;

	dir_x = 0;
	dir_y = 0;
	if (game->p1.keys.w)
	{
		dir_x += game->p1.dir.x;
		dir_y += game->p1.dir.y;
	}
	if (game->p1.keys.s)
	{
		dir_x -= game->p1.dir.x;
		dir_y -= game->p1.dir.y;
	}
	keys_else(game, &dir_x, &dir_y);
	move_player(game, dir_x, dir_y);
	if (game->p1.keys.left)
		rotate_player(game, game->p1.rot_speed);
	if (game->p1.keys.right)
		rotate_player(game, -game->p1.rot_speed);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		close_window(game);
	else if (keycode == KEY_W)
		game->p1.keys.w = 1;
	else if (keycode == KEY_S)
		game->p1.keys.s = 1;
	else if (keycode == KEY_D)
		game->p1.keys.d = 1;
	else if (keycode == KEY_A)
		game->p1.keys.a = 1;
	else if (keycode == KEY_LEFT)
		game->p1.keys.left = 1;
	else if (keycode == KEY_RIGHT)
		game->p1.keys.right = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->p1.keys.w = 0;
	else if (keycode == KEY_S)
		game->p1.keys.s = 0;
	else if (keycode == KEY_D)
		game->p1.keys.d = 0;
	else if (keycode == KEY_A)
		game->p1.keys.a = 0;
	else if (keycode == KEY_LEFT)
		game->p1.keys.left = 0;
	else if (keycode == KEY_RIGHT)
		game->p1.keys.right = 0;
	return (0);
}
