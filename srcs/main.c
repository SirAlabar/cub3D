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
static void setup_hooks(t_game *game)
{
    mlx_hook(game->win, 2, 1L << 0, key_press, game);
    mlx_hook(game->win, 3, 1L << 1, key_release, game);
    mlx_hook(game->win, 17, 0, close_window, game);
    mlx_hook(game->win, 6, 1L << 6, mouse_wrapper, game);
    mlx_loop_hook(game->mlx, render_frame, game);
}
int main(int argc, char **argv)
{
    t_game *game;

    game = ft_calloc(1, sizeof(t_game));
    if (!game)
        return (1);
    if (!validate_map_extension(argc, argv))
        return (cleanup_game(game), 1);
    game->map = ft_calloc(1, (sizeof(t_doom_map)));
    if (!game->map || !load_map(argc, argv, game->map))
        return (cleanup_game(game), 1);
    game->bsp_tree = init_bsp_build(game->map);
    if (!game->bsp_tree)
        return (cleanup_game(game), 1);
    if (!init_window(game))
        return (cleanup_game(game), 1);
    init_game(game);
    setup_hooks(game);
    mlx_loop(game->mlx);
    cleanup_game(game);
    return (0);
}

