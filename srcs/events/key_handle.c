/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/09 12:12:34 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	handle_movement_keys(int keycode, t_game *game)
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
}

void	handle_action_keys(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		close_window(game);
	else if (keycode == KEY_E)
		interact_with_door(game);
	else if (keycode == KEY_SPACE || keycode == MOUSE_LEFT)
	{
		game->p1.is_firing = 1;
		game->p1.current_frame = 1;
		game->p1.last_fire = get_time_ms();
		shoot_enemy(game);
	}
}

int	key_press(int keycode, t_game *game)
{
	handle_movement_keys(keycode, game);
	handle_action_keys(keycode, game);
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
