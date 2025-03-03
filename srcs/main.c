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
/*
** Sets up MLX window and initializes double buffer
** Returns false if any initialization fails
*/
bool	init_window(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (false);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "BSP Doom");
	if (!game->win)
		return (false);
	init_double_buffer(game);
	if (!game->buffer[0] || !game->buffer[1])
		return (false);
	return (true);
}
static void setup_hooks(t_game *game)
{
    mlx_hook(game->win, 2, 1L << 0, handle_key_press, game);
    mlx_hook(game->win, 3, 1L << 1, handle_key_release, game);
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
    if (!init_window(game))
        return (cleanup_game(game), 1);
    if (!init_game(game))
        return (cleanup_game(game), 1);
    game->bsp_tree = init_bsp_build(game->map, game->thread_pool);
    if (!game->bsp_tree)
        return (cleanup_game(game), 1);
    setup_hooks(game);
    mlx_loop(game->mlx);
    cleanup_game(game);
    return (0);
}

