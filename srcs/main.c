/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/24 19:13:09 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	close_window(t_game *game)
{
	mlx_loop_end(game->mlx);
	return (0);
}

int	mouse_press(int button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (!game)
		return (0);
	if (button == MOUSE_LEFT && game->active_weapon == 0)
	{
		game->p1.is_firing = 1;
		if (game->sounds && game->sounds->is_initialized)
			play_sound(game->sounds->gun);
	}
	else if (button == MOUSE_LEFT && game->active_weapon == 1
		&& game->portal_system)
	{
		handle_portal_gun_input(game, button);
		if (game->sounds && game->sounds->is_initialized)
			play_sound(game->sounds->portal);
	}
	else if (button == MOUSE_RIGHT && game->portal_system
		&& game->active_weapon == 1)
	{
		handle_portal_gun_input(game, button);
		if (game->sounds && game->sounds->is_initialized)
			play_sound(game->sounds->portal);
	}
	return (0);
}

bool	init_window(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (false);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->win)
		return (false);
	init_double_buffer(game);
	if (!game->img[0] || !game->img[1])
		return (false);
	return (true);
}

void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, NULL, NULL);
	mlx_hook(game->win, 3, 1L << 1, NULL, NULL);
	mlx_hook(game->win, 6, 1L << 6, NULL, NULL);
	mlx_hook(game->win, 4, 1L << 2, NULL, NULL);
	mlx_loop_hook(game->mlx, NULL, NULL);
	mlx_hook(game->win, 17, 0, close_window, game);
	if (game->menu && game->menu->active)
	{
		mlx_hook(game->win, 2, 1L << 0, menu_key_press, game);
		mlx_loop_hook(game->mlx, menu_render_frame, game);
	}
	else
	{
		mlx_hook(game->win, 2, 1L << 0, key_press, game);
		mlx_hook(game->win, 3, 1L << 1, key_release, game);
		mlx_hook(game->win, 6, 1L << 6, mouse_wrapper, game);
		mlx_hook(game->win, 4, 1L << 2, mouse_press, game);
		mlx_loop_hook(game->mlx, engine_render_frame, game);
	}
}

int	main(int argc, char **argv)
{
	t_game	*game;

	srand(time(NULL));
	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (1);
	if (!validate_map_extension(argc, argv))
		return (free(game), 1);
	if (!checker(game, argv[1]))
		return (1);
	if (!init_window(game))
	{
		cleanup_game(game);
		return (1);
	}
	init_start_menu(game);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}
