/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 18:55:52 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static int	handle_key_weapons(int keycode, t_game *game)
{
	if (keycode == KEY_SPACE)
	{
		if (game->active_weapon == 0)
			game->p1.is_firing = 1;
		if (game->sounds && game->sounds->is_initialized)
			play_sound(game->sounds->gun);
		return (1);
	}
	else if (keycode == KEY_Q)
	{
		switch_weapon(game);
		return (1);
	}
	return (0);
}

static int	handle_key_movement(int keycode, t_game *game)
{
	if (keycode == KEY_W)
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
	else if (keycode == KEY_LSHIFT || keycode == KEY_RSHIFT)
		game->p1.keys.shift = 1;
	else
		return (0);
	return (1);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		close_window(game);
	else if (keycode == KEY_E)
		interact_with_door(game);
	else if (handle_key_movement(keycode, game))
		return (0);
	else if (handle_key_weapons(keycode, game))
		return (0);
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
	else if (keycode == KEY_LSHIFT || keycode == KEY_RSHIFT)
		game->p1.keys.shift = 0;
	return (0);
}
