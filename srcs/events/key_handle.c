/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 20:13:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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
		rotate_player(game, -game->p1.rot_speed);
	if (game->p1.keys.right)
		rotate_player(game, game->p1.rot_speed);
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
