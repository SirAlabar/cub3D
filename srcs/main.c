/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/11 13:48:06 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	close_window(t_game *game)
{
	mlx_loop_end(game->mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	ft_bzero(game, sizeof(t_game));
	if (!game)
		return (1);
	if (!validate_map_extension(argc, argv))
		return (free(game), 1);
	if (!checker(game, argv[1]))
		return (1);
	game->mlx = mlx_init();
	if (!game->mlx)
		return (cleanup_game(game), 1);
	init_game(game);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->win)
		return (cleanup_game(game), 1);
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_hook(game->win, 6, 1L << 6, mouse_wrapper, game);
	mlx_loop_hook(game->mlx, engine_render_frame, game);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}
