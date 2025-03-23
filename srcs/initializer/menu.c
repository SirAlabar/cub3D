/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:37:47 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 18:37:47 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	menu_render_frame(t_game *game)
{
	if (!game || !game->addr[game->current_buffer])
		return (0);
	draw_menu(game);
	swap_buffers(game);
	return (0);
}

void	setup_menu_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, menu_key_press, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, menu_render_frame, game);
}

int	init_menu(t_game *game)
{
	game->menu = ft_calloc(1, sizeof(t_menu));
	if (!game->menu)
		return (0);
	game->menu->active = 1;
	game->menu->selected_option = 0;
	if (!init_menu_background(game) || !init_menu_cube(game)
		|| !init_menu_start(game) || !init_menu_exit(game)
		|| !init_select_gun(game))
	{
		cleanup_menu(game);
		return (0);
	}
	return (1);
}
