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
#include "bsp.h"
#include "map.h"
#include <stdio.h>

/* Helper function to print fixed point vector */
static void print_fixed_vec(const char *prefix, t_fixed_vec32 vec)
{
    printf("%s(%.2f, %.2f)\n", prefix,
        (float)vec.x / 65536.0f,
        (float)vec.y / 65536.0f);
}

/* Creates a test map with a simple square room */
static t_doom_map *create_test_map(void)
{
    t_doom_map *map;
    
    map = calloc(1, sizeof(t_doom_map));
    if (!map)
        return (NULL);

    /* Create vertices for a square room */
    map->vertices[0] = (t_vertex){(t_fixed_vec32){0, 0}};         // Top-left
    map->vertices[1] = (t_vertex){(t_fixed_vec32){65536, 0}};     // Top-right
    map->vertices[2] = (t_vertex){(t_fixed_vec32){65536, 65536}}; // Bottom-right
    map->vertices[3] = (t_vertex){(t_fixed_vec32){0, 65536}};     // Bottom-left
    map->vertex_count = 4;

    /* Create linedefs forming the walls */
    map->linedefs[0] = (t_linedef){0, 1, -1, -1, 0, 0};  // Top wall
    map->linedefs[1] = (t_linedef){1, 2, -1, -1, 0, 0};  // Right wall
    map->linedefs[2] = (t_linedef){2, 3, -1, -1, 0, 0};  // Bottom wall
    map->linedefs[3] = (t_linedef){3, 0, -1, -1, 0, 0};  // Left wall
    map->linedef_count = 4;

    return (map);
}

/* Test BSP tree creation and structure */
static void test_bsp_tree_creation(void)
{
    t_doom_map *map;
    t_bsp_tree *tree;

    printf("\n=== Testing BSP Tree Creation ===\n");
    
    map = create_test_map();
    if (!map)
    {
        printf("Failed to create test map\n");
        return;
    }

    tree = init_bsp_build(map);
    if (!tree)
    {
        printf("Failed to initialize BSP tree\n");
        free(map);
        return;
    }

    printf("\nInitial tree structure:\n");
    print_bsp_tree(tree);

    printf("\nBalancing tree...\n");
    if (balance_bsp_tree(tree))
        printf("Tree balanced successfully\n");
    else
        printf("Failed to balance tree\n");

    print_bsp_tree(tree);

    free_bsp_tree(tree);
    free(map);
}

/* Test collision detection and point classification */
static void test_collision_detection(void)
{
    t_doom_map *map;
    t_bsp_tree *tree;
    t_fixed_vec32 test_points[] = {
        {32768, 32768},  // Center (0.5, 0.5)
        {16384, 16384},  // Near corner (0.25, 0.25)
        {49152, 49152}   // Near corner (0.75, 0.75)
    };
    
    printf("\n=== Testing Collision Detection ===\n");
    
    map = create_test_map();
    if (!map || !(tree = init_bsp_build(map)))
    {
        printf("Failed to initialize test\n");
        free(map);
        return;
    }

    /* Test each point */
    for (int i = 0; i < 3; i++)
    {
        print_fixed_vec("\nTesting point: ", test_points[i]);
        
        t_fixed32 dist = find_nearest_wall(tree->root, test_points[i]);
        printf("Distance to nearest wall: %.2f units\n", 
            (float)dist / 65536.0f);
        
        /* Test movement in different directions */
        t_fixed_vec32 movements[] = {
            {6554, 0},     // Right (0.1, 0)
            {0, 6554},     // Down (0, 0.1)
            {-6554, 0},    // Left (-0.1, 0)
            {0, -6554}     // Up (0, -0.1)
        };
        
        for (int j = 0; j < 4; j++)
        {
            t_fixed_vec32 end = {
                fixed32_add(test_points[i].x, movements[j].x),
                fixed32_add(test_points[i].y, movements[j].y)
            };
            
            print_fixed_vec("Testing movement to: ", end);
            if (check_movement_valid(tree, test_points[i], end))
                printf("Movement is valid\n");
            else
                printf("Movement would cause collision\n");
        }
    }

    free_bsp_tree(tree);
    free(map);
}

/* Main test function */
int main(void)
{
    test_bsp_tree_creation();
    test_collision_detection();
    
    printf("\nBSP tests completed.\n");
    return (0);
}