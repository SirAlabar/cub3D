/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:11:37 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/22 20:01:42 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	handle_firing(t_game *game)
{
	game->p1.is_firing = 1;
	shoot_enemy(game);
	if (game->sounds && game->sounds->is_initialized)
		play_sound(game->sounds->gun);
}

static void	handle_menu_selection(t_game *game)
{
	if (game->menu->selected_option == 0)
	{
		game->menu->active = 0;
		mlx_hook(game->win, 2, 1L << 0, NULL, NULL);
		mlx_loop_hook(game->mlx, NULL, NULL);
		init_game(game);
		setup_hooks(game);
		cleanup_menu(game);
	}
	else
	{
		cleanup_menu(game);
		close_window(game);
	}
}

int	menu_key_press(int keycode, t_game *game)
{
	if (!game->menu->active)
		return (0);
	if (keycode == KEY_ESC)
		close_window(game);
	else if (keycode == KEY_UP || keycode == KEY_W)
		game->menu->selected_option = 0;
	else if (keycode == KEY_DOWN || keycode == KEY_S)
		game->menu->selected_option = 1;
	else if (keycode == KEY_ENTER || keycode == KEY_SPACE)
		handle_menu_selection(game);
	return (0);
}
