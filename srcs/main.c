/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/01 16:36:27 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void cleanup_game(t_game *game)
{
	if (!game)
		return;
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
	free(game);
}

int key_handler(int keycode, t_game *game)
{
	(void) game;
	if (keycode == KEY_ESC)
	{
		cleanup_game(game);
		exit(0);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_game	*game;
	
	game = malloc(sizeof(t_game));
	if (!game)
		return (1);
	if (!validate_map_extension(argc, argv))
		return (free(game), 1);
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, 800, 600, "My new cube omg");

	render_frame(game);
	mlx_hook(game->win, 2, 1L<<0, key_handler, game);
	game_parse(game, argv[1]);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}
