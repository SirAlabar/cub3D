/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 18:00:39 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int close_window(t_game *game)
{
    if (game->img)
        mlx_destroy_image(game->mlx, game->img);
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
    exit(0);
    return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	init_game(&game);
	if (!validate_map_extension(argc, argv))
	return (1);
    game.mlx = mlx_init();
    if (!game.mlx)
        return (1);
    game.win = mlx_new_window(game.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game.win)
    {
        mlx_destroy_display(game.mlx);
        free(game.mlx);
        return (1);
    }
	game.img = mlx_new_image(game.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game.img)
    {
        mlx_destroy_window(game.mlx, game.win);
        mlx_destroy_display(game.mlx);
        free(game.mlx);
        return (1);
	}

    game.addr = mlx_get_data_addr(game.img, &game.north.bpp, 
                                 &game.north.line_len, &game.north.endian);

	mlx_hook(game.win, 17, 0 , close_window, &game);
	mlx_loop(game.mlx);							 
	return (0);
}
