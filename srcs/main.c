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
    t_game      *game;
    t_doom_map  *map;
    t_bsp_tree  *tree;

    game = malloc(sizeof(t_game));
    ft_bzero(game, sizeof(t_game));
    if (!game)
        return (1);

    // Carregar o mapa a partir de um arquivo
    map = malloc(sizeof(t_doom_map));
    if (!map)
        return (1);
    if (!load_map(argc, argv, map))
    {
        free(map);
        return (1);
    }

    // Construir a árvore BSP a partir do mapa
    tree = init_bsp_build(map);
    if (!tree)
    {
        free(map);
        return (1);
    }

    // Inicializar a janela do jogo
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "BSP Visualization");
    if (!game->win)
    {
        //cleanup_game(game);
        free_bsp_tree(tree);
        free(map);
        return (1);
    }

    // Atribuir o mapa e a árvore BSP ao jogo
    game->map = map;
    game->bsp_tree = tree;

    // Iniciar o loop do jogo
    mlx_loop_hook(game->mlx, &render_frame, game);
    mlx_loop(game->mlx);

    // Liberar recursos
    //cleanup_game(game);
    free_bsp_tree(tree);
    free(map);

    return (0);
}

