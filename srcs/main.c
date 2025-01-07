/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/07 12:38:34 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cleanup_game(t_game *game)
{
	int	i;

	if (!game)
		return ;
	if (game->win && game->mlx)
		destroy_window(game->mlx, game->win);
	if (game->img && game->mlx)
		mlx_destroy_image(game->mlx, game->img);
	if (game->mlx)
		cleanup_mlx(game->mlx);
	if (game->map.grid)
	{
		i = 0;
		while (game->map.grid[i])
			free(game->map.grid[i++]);
		free(game->map.grid);
	}
	free(game);
}

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
	init_game(game);
	game->mlx = mlx_init();
	if (!game->mlx)
		return (cleanup_game(game), 1);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->win)
		return (cleanup_game(game), 1);
	mlx_hook(game->win, 2, 1L << 0, key_handler, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, engine_render_frame, game);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}
