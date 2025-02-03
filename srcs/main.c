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
/*
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
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_fixed_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:06:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/30 15:08:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fixed_point.h"
#include <cub3d.h>
#include <stdio.h>
#include <math.h>
#include <map.h>

/*
** Create a simple test map with known geometry
*/
static t_bsp_line	**create_test_lines(int *num_lines)
{
	t_bsp_line	**lines;

	*num_lines = 4;
	lines = malloc(sizeof(t_bsp_line *) * *num_lines);
	if (!lines)
		return (NULL);
	// Create a simple square
	lines[0] = create_bsp_line((t_fixed_vec32){0, 0},
			(t_fixed_vec32){65536, 0}, 0);  // Top (1.0 in fixed point)
	lines[1] = create_bsp_line((t_fixed_vec32){65536, 0},
			(t_fixed_vec32){65536, 65536}, 0);  // Right
	lines[2] = create_bsp_line((t_fixed_vec32){65536, 65536},
			(t_fixed_vec32){0, 65536}, 0);  // Bottom
	lines[3] = create_bsp_line((t_fixed_vec32){0, 65536},
			(t_fixed_vec32){0, 0}, 0);  // Left
	return (lines);
}

/*
** Test BSP tree creation and balancing
*/
static void	test_bsp_creation(void)
{
	t_bsp_line	**lines;
	t_bsp_tree	*tree;
	int			num_lines;

	ft_printf("\n=== Testing BSP Creation ===\n");
	lines = create_test_lines(&num_lines);
	if (!lines)
	{
		ft_printf("Failed to create test lines\n");
		return ;
	}
	tree = malloc(sizeof(t_bsp_tree));
	if (!tree)
	{
		ft_printf("Failed to allocate tree\n");
		return ;
	}
	tree->root = build_bsp_tree(lines, num_lines);
	if (!tree->root)
	{
		ft_printf("Failed to build BSP tree\n");
		free(tree);
		return ;
	}
	ft_printf("\nInitial BSP Tree:\n");
	print_bsp_tree(tree);
	ft_printf("\nBalancing tree...\n");
	if (balance_bsp_tree(tree))
	{
		ft_printf("\nBalanced BSP Tree:\n");
		print_bsp_tree(tree);
	}
	else
		ft_printf("Failed to balance tree\n");
	free_bsp_tree(tree);
}

/*
** Test collision detection
*/
static void	test_collisions(void)
{
	t_bsp_line	**lines;
	t_bsp_tree	*tree;
	int			num_lines;
	t_fixed_vec32	test_point;
	t_fixed_vec32	movement;

	ft_printf("\n=== Testing Collisions ===\n");
	lines = create_test_lines(&num_lines);
	if (!lines)
		return ;
	tree = malloc(sizeof(t_bsp_tree));
	if (!tree)
		return ;
	tree->root = build_bsp_tree(lines, num_lines);
	if (!tree->root)
		return ;
	
	// Test point inside square
	test_point = (t_fixed_vec32){32768, 32768};  // (0.5, 0.5)
	ft_printf("\nTesting point (0.5, 0.5):\n");
	ft_printf("Distance to nearest wall: %d\n",
		fixed32_to_int(find_nearest_wall(tree->root, test_point)));

	// Test movement
	movement = (t_fixed_vec32){65536, 0};  // Try to move right by 1.0
	ft_printf("\nTesting movement right by 1.0:\n");
	if (check_movement_valid(tree, test_point,
			(t_fixed_vec32){98304, 32768}))  // (1.5, 0.5)
		ft_printf("Movement valid\n");
	else
		ft_printf("Movement blocked\n");

	adjust_collision_response(tree->root, &movement);
	ft_printf("Adjusted movement: (%d, %d)\n",
		fixed32_to_int(movement.x),
		fixed32_to_int(movement.y));

	free_bsp_tree(tree);
}

int	main(void)
{
	ft_printf("\n=== BSP Tree Test Program ===\n");
	
	test_bsp_creation();
	test_collisions();

	ft_printf("\n=== End of Tests ===\n");
	return (0);
}