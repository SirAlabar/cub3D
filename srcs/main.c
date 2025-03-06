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

int mouse_press(int button, int x, int y, t_game *game)
{
    (void)x; // Para evitar warnings de variáveis não utilizadas
    (void)y;
    
    printf("Mouse button pressed: %d at position (%d, %d)\n", button, x, y);
    
    if (!game || !game->portal_system)
        return (0);
        
	else if (button == MOUSE_LEFT || button == MOUSE_RIGHT)
	{
		printf("DEBUG: game pointer = %p\n", (void*)game);
		printf("DEBUG: portal_system pointer = %p\n", (void*)game->portal_system);
		if (game->portal_system)
			handle_portal_gun_input(game, button);
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

static void	setup_hooks(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_hook(game->win, 6, 1L << 6, mouse_wrapper, game);
	mlx_hook(game->win, 4, 1L << 2, mouse_press, game);
	mlx_loop_hook(game->mlx, engine_render_frame, game);
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
	game->mlx = mlx_init();
	if (!game->mlx)
		return (cleanup_game(game), 1);
	if (!init_window(game))
	{
		cleanup_game(game);
		return (1);
	}
	init_game(game);
	setup_hooks(game);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}
