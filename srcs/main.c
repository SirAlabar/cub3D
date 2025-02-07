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

#include <stdio.h>
#include "fixed_point.h"
#include "bsp.h"
#include "map.h"

static void print_map_details(t_doom_map *map)
{
    ft_printf("\n=== Map Details ===\n");
    
    // Vertices
    ft_printf("Vertices (%d):\n", map->vertex_count);
    for (int i = 0; i < map->vertex_count; i++) {
        ft_printf("v%d: (%d, %d)\n", 
            i,
            fixed32_to_int(map->vertices[i].pos.x),
            fixed32_to_int(map->vertices[i].pos.y)
        );
    }

    // Linedefs
    ft_printf("\nLinedefs (%d):\n", map->linedef_count);
    for (int i = 0; i < map->linedef_count; i++) {
        ft_printf("l%d: v%d -> v%d (type: %d, front_sidedef: %d, back_sidedef: %d)\n", 
            i, 
            map->linedefs[i].vertex1, 
            map->linedefs[i].vertex2,
            map->linedefs[i].type,
            map->linedefs[i].front_sidedef,
            map->linedefs[i].back_sidedef
        );
    }

    ft_printf("\n=== End of Map Details ===\n");
}


/* Creates a test map with a simple square room */
static t_doom_map *create_complex_test_map(void)
{
    t_doom_map *map = ft_calloc(1, sizeof(t_doom_map));
    if (!map) return NULL;

    // Vertices
    int vertex_coords[23][2] = {
        {0, 0}, {256, 0}, {256, 256}, {0, 256},           // Sala 1 Lab
        {320, 0}, {512, 0}, {512, 256}, {320, 256},        // Sala 2 Metal
        {0, 320}, {256, 320}, {256, 512}, {0, 512},        // Sala 3 Ice
        {320, 320}, {512, 320}, {512, 512}, {320, 512},    // Sala 4 Fire
        {128, 128}, {192, 64}, {256, 128}, {192, 192},     // Sala Octagonal
        {128, 192}, {64, 128}, {128, 64}                   // Sala Octagonal continuação
    };

    // Adicionar vértices
    for (int i = 0; i < 23; i++) {
        map->vertices[i] = (t_vertex){
            (t_fixed_vec32){
                int_to_fixed32(vertex_coords[i][0]), 
                int_to_fixed32(vertex_coords[i][1])
            }
        };
    }
    map->vertex_count = 23;

    // Linhas
    int linedef_data[26][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},      // Sala 1 Lab
        {4, 5}, {5, 6}, {6, 7}, {7, 4},      // Sala 2 Metal
        {8, 9}, {9, 10}, {10, 11}, {11, 8},  // Sala 3 Ice
        {12, 13}, {13, 14}, {14, 15}, {15, 12}, // Sala 4 Fire
        {16, 17}, {17, 18}, {18, 19}, {19, 20}, // Sala Octagonal
        {20, 21}, {21, 22}, {22, 16},
        {1, 4}, {2, 9}, {7, 12}             // Portas entre salas
    };

    for (int i = 0; i < 26; i++) {
        map->linedefs[i] = (t_linedef){
            linedef_data[i][0], 
            linedef_data[i][1], 
            (i < 24) ? -1 : (i - 24), // Identificadores de porta
            -1, 0, 0
        };
    }
    map->linedef_count = 26;

    return map;
}

static void test_map_line_extraction(void)
{
    t_doom_map *map = create_complex_test_map();
    t_bsp_line **lines;
    int num_lines;

    print_map_details(map);

    ft_printf("\n\n\n=== Testing Map Line Extraction ===\n");

    if (!map) {
        ft_printf("Failed to create test map\n");
        return;
    }

    if (extract_map_lines(map, &lines, &num_lines)) {
        ft_printf("Successfully extracted %d lines\n", num_lines);
        
        // Print first few line details
        for (int i = 0; i < (num_lines < 5 ? num_lines : 5); i++) {
            ft_printf("Line %d: (%d,%d) -> (%d,%d) [type: %d]\n", 
                i,
                fixed32_to_int(lines[i]->start.x),
                fixed32_to_int(lines[i]->start.y),
                fixed32_to_int(lines[i]->end.x),
                fixed32_to_int(lines[i]->end.y),
                lines[i]->type
            );
        }

        // Free lines
        for (int i = 0; i < num_lines; i++) {
            free(lines[i]);
        }
        free(lines);
    } else {
        ft_printf("Failed to extract map lines\n");
    }

    free(map);
}

static void test_line_classification(void)
{
    t_doom_map *map = create_complex_test_map();
    t_bsp_line **lines;
    int num_lines;

    ft_printf("\n=== Testing Line Classification ===\n");

    if (!map || !extract_map_lines(map, &lines, &num_lines)) {
        ft_printf("Failed to prepare test data\n");
        free(map);
        return;
    }

    // Choose a partition line (first line)
    t_bsp_line *partition = lines[0];

    ft_printf("Partition Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(partition->start.x),
        fixed32_to_int(partition->start.y),
        fixed32_to_int(partition->end.x),
        fixed32_to_int(partition->end.y)
    );

    // Classify other lines against this partition
    for (int i = 1; i < num_lines; i++) {
        t_bsp_side side = bsp_classify_line(lines[i], partition);
        
        ft_printf("Line %d: (%d,%d) -> (%d,%d) - Side: %d\n", 
            i,
            fixed32_to_int(lines[i]->start.x),
            fixed32_to_int(lines[i]->start.y),
            fixed32_to_int(lines[i]->end.x),
            fixed32_to_int(lines[i]->end.y),
            side
        );
    }

    // Free lines
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(map);
}

static void test_partition_selection(void)
{
    t_doom_map *map = create_complex_test_map();
    t_bsp_line **lines;
    int num_lines;

    ft_printf("\n=== Testing Partition Selection ===\n");

    if (!map || !extract_map_lines(map, &lines, &num_lines)) {
        ft_printf("Failed to prepare test data\n");
        free(map);
        return;
    }

    t_bsp_line *best_partition = choose_partition(lines, num_lines);

    if (best_partition) {
        ft_printf("Best Partition Selected: (%d,%d) -> (%d,%d)\n",
            fixed32_to_int(best_partition->start.x),
            fixed32_to_int(best_partition->start.y),
            fixed32_to_int(best_partition->end.x),
            fixed32_to_int(best_partition->end.y)
        );
    } else {
        ft_printf("No partition could be selected\n");
    }

    // Free lines
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(map);
}

static void test_point_classification(void)
{
    ft_printf("\n=== Testing Point Classification ===\n");

    // Criar uma linha de teste horizontal
    t_bsp_line test_line = {
        .start = {int_to_fixed32(0), int_to_fixed32(0)},
        .end = {int_to_fixed32(100), int_to_fixed32(0)}
    };

    ft_printf("Test Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(test_line.start.x),
        fixed32_to_int(test_line.start.y),
        fixed32_to_int(test_line.end.x),
        fixed32_to_int(test_line.end.y));

    // Teste pontos claramente de um lado ou outro
    t_fixed_vec32 point_front = {int_to_fixed32(50), int_to_fixed32(10)};
    t_fixed_vec32 point_back = {int_to_fixed32(50), int_to_fixed32(-10)};
    
    ft_printf("\nTesting clear front/back points:\n");
    ft_printf("Point Front (%d,%d): %d\n", 
        fixed32_to_int(point_front.x), 
        fixed32_to_int(point_front.y),
        bsp_classify_point(point_front, &test_line));
    ft_printf("Point Back (%d,%d): %d\n",
        fixed32_to_int(point_back.x),
        fixed32_to_int(point_back.y),
        bsp_classify_point(point_back, &test_line));

    // Teste pontos próximos à linha
    t_fixed_vec32 point_near = {int_to_fixed32(50), int_to_fixed32(0)};
    ft_printf("\nTesting point on the line:\n");
    ft_printf("Point Near (%d,%d): %d\n",
        fixed32_to_int(point_near.x),
        fixed32_to_int(point_near.y),
        bsp_classify_point(point_near, &test_line));

    // Teste pontos além dos endpoints
    t_fixed_vec32 point_beyond = {int_to_fixed32(150), int_to_fixed32(0)};
    ft_printf("\nTesting point beyond endpoint:\n");
    ft_printf("Point Beyond (%d,%d): %d\n",
        fixed32_to_int(point_beyond.x),
        fixed32_to_int(point_beyond.y),
        bsp_classify_point(point_beyond, &test_line));

    // Teste com linha diagonal
    t_bsp_line diagonal_line = {
        .start = {int_to_fixed32(0), int_to_fixed32(0)},
        .end = {int_to_fixed32(100), int_to_fixed32(100)}
    };

    t_fixed_vec32 diagonal_test = {int_to_fixed32(50), int_to_fixed32(40)};
    
    ft_printf("\nTesting with diagonal line:\n");
    ft_printf("Diagonal Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(diagonal_line.start.x),
        fixed32_to_int(diagonal_line.start.y),
        fixed32_to_int(diagonal_line.end.x),
        fixed32_to_int(diagonal_line.end.y));
    ft_printf("Test Point (%d,%d): %d\n",
        fixed32_to_int(diagonal_test.x),
        fixed32_to_int(diagonal_test.y),
        bsp_classify_point(diagonal_test, &diagonal_line));

    ft_printf("\n=== End of Point Classification Tests ===\n");
}

static void test_line_splitting(void)
{
    ft_printf("\n=== Testing Line Splitting ===\n");

    // Caso 1: Split perpendicular básico
    t_bsp_line partition1 = {
        .start = {int_to_fixed32(0), int_to_fixed32(100)},
        .end = {int_to_fixed32(200), int_to_fixed32(100)},
        .type = 0
    };

    t_bsp_line test_line1 = {
        .start = {int_to_fixed32(100), int_to_fixed32(0)},
        .end = {int_to_fixed32(100), int_to_fixed32(200)},
        .type = 0
    };

    ft_printf("\nTest 1 - Basic Perpendicular Split:\n");
    ft_printf("Partition: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(partition1.start.x),
        fixed32_to_int(partition1.start.y),
        fixed32_to_int(partition1.end.x),
        fixed32_to_int(partition1.end.y));
    ft_printf("Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(test_line1.start.x),
        fixed32_to_int(test_line1.start.y),
        fixed32_to_int(test_line1.end.x),
        fixed32_to_int(test_line1.end.y));

    t_bsp_line *front_split;
    t_bsp_line *back_split;

    if (split_bsp_line(&test_line1, &partition1, &front_split, &back_split))
    {
        ft_printf("Split successful!\n");
        if (front_split)
            ft_printf("Front part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(front_split->start.x),
                fixed32_to_int(front_split->start.y),
                fixed32_to_int(front_split->end.x),
                fixed32_to_int(front_split->end.y));
        if (back_split)
            ft_printf("Back part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(back_split->start.x),
                fixed32_to_int(back_split->start.y),
                fixed32_to_int(back_split->end.x),
                fixed32_to_int(back_split->end.y));
        free(front_split);
        free(back_split);
    }
    else
        ft_printf("Split failed!\n");

    // Caso 2: Split diagonal em 45 graus
    t_bsp_line diagonal_partition = {
        .start = {int_to_fixed32(0), int_to_fixed32(0)},
        .end = {int_to_fixed32(100), int_to_fixed32(100)},
        .type = 0
    };

    t_bsp_line diagonal_test = {
        .start = {int_to_fixed32(0), int_to_fixed32(100)},
        .end = {int_to_fixed32(100), int_to_fixed32(0)},
        .type = 0
    };

    ft_printf("\nTest 2 - 45-Degree Diagonal Split:\n");
    ft_printf("Partition: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(diagonal_partition.start.x),
        fixed32_to_int(diagonal_partition.start.y),
        fixed32_to_int(diagonal_partition.end.x),
        fixed32_to_int(diagonal_partition.end.y));
    ft_printf("Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(diagonal_test.start.x),
        fixed32_to_int(diagonal_test.start.y),
        fixed32_to_int(diagonal_test.end.x),
        fixed32_to_int(diagonal_test.end.y));

    if (split_bsp_line(&diagonal_test, &diagonal_partition, &front_split, &back_split))
    {
        ft_printf("Split successful!\n");
        if (front_split)
            ft_printf("Front part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(front_split->start.x),
                fixed32_to_int(front_split->start.y),
                fixed32_to_int(front_split->end.x),
                fixed32_to_int(front_split->end.y));
        if (back_split)
            ft_printf("Back part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(back_split->start.x),
                fixed32_to_int(back_split->start.y),
                fixed32_to_int(back_split->end.x),
                fixed32_to_int(back_split->end.y));
        free(front_split);
        free(back_split);
    }
    else
        ft_printf("Split failed!\n");

    // Caso 3: Linhas paralelas horizontais
    t_bsp_line parallel_test = {
        .start = {int_to_fixed32(0), int_to_fixed32(150)},
        .end = {int_to_fixed32(200), int_to_fixed32(150)},
        .type = 0
    };

    ft_printf("\nTest 3 - Parallel Lines:\n");
    ft_printf("Testing parallel line with horizontal partition:\n");
    ft_printf("Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(parallel_test.start.x),
        fixed32_to_int(parallel_test.start.y),
        fixed32_to_int(parallel_test.end.x),
        fixed32_to_int(parallel_test.end.y));

    if (split_bsp_line(&parallel_test, &partition1, &front_split, &back_split))
    {
        ft_printf("Split occurred (unexpected!)\n");
        free(front_split);
        free(back_split);
    }
    else
        ft_printf("No split (correct for parallel lines)\n");

    // Caso 4: Split em ângulo pequeno
    t_bsp_line steep_partition = {
        .start = {int_to_fixed32(0), int_to_fixed32(0)},
        .end = {int_to_fixed32(10), int_to_fixed32(100)},
        .type = 0
    };

    t_bsp_line steep_test = {
        .start = {int_to_fixed32(0), int_to_fixed32(100)},
        .end = {int_to_fixed32(10), int_to_fixed32(0)},
        .type = 0
    };

    ft_printf("\nTest 4 - Steep Angle Split:\n");
    ft_printf("Partition: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(steep_partition.start.x),
        fixed32_to_int(steep_partition.start.y),
        fixed32_to_int(steep_partition.end.x),
        fixed32_to_int(steep_partition.end.y));
    ft_printf("Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(steep_test.start.x),
        fixed32_to_int(steep_test.start.y),
        fixed32_to_int(steep_test.end.x),
        fixed32_to_int(steep_test.end.y));

    if (split_bsp_line(&steep_test, &steep_partition, &front_split, &back_split))
    {
        ft_printf("Split successful!\n");
        if (front_split)
            ft_printf("Front part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(front_split->start.x),
                fixed32_to_int(front_split->start.y),
                fixed32_to_int(front_split->end.x),
                fixed32_to_int(front_split->end.y));
        if (back_split)
            ft_printf("Back part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(back_split->start.x),
                fixed32_to_int(back_split->start.y),
                fixed32_to_int(back_split->end.x),
                fixed32_to_int(back_split->end.y));
        free(front_split);
        free(back_split);
    }
    else
        ft_printf("Split failed!\n");

    // Caso 5: Linhas muito próximas do fim
    t_bsp_line edge_partition = {
        .start = {int_to_fixed32(100), int_to_fixed32(0)},
        .end = {int_to_fixed32(100), int_to_fixed32(100)},
        .type = 0
    };

    t_bsp_line edge_test = {
        .start = {int_to_fixed32(99), int_to_fixed32(50)},
        .end = {int_to_fixed32(101), int_to_fixed32(50)},
        .type = 0
    };

    ft_printf("\nTest 5 - Edge Case Split:\n");
    ft_printf("Partition: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(edge_partition.start.x),
        fixed32_to_int(edge_partition.start.y),
        fixed32_to_int(edge_partition.end.x),
        fixed32_to_int(edge_partition.end.y));
    ft_printf("Line: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(edge_test.start.x),
        fixed32_to_int(edge_test.start.y),
        fixed32_to_int(edge_test.end.x),
        fixed32_to_int(edge_test.end.y));

    if (split_bsp_line(&edge_test, &edge_partition, &front_split, &back_split))
    {
        ft_printf("Split successful!\n");
        if (front_split)
            ft_printf("Front part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(front_split->start.x),
                fixed32_to_int(front_split->start.y),
                fixed32_to_int(front_split->end.x),
                fixed32_to_int(front_split->end.y));
        if (back_split)
            ft_printf("Back part: (%d,%d) -> (%d,%d)\n",
                fixed32_to_int(back_split->start.x),
                fixed32_to_int(back_split->start.y),
                fixed32_to_int(back_split->end.x),
                fixed32_to_int(back_split->end.y));
        free(front_split);
        free(back_split);
    }
    else
        ft_printf("Split failed!\n");

    ft_printf("\n=== End of Line Splitting Tests ===\n");
}

int main(void)
{
    test_map_line_extraction();
    test_line_classification();
    test_partition_selection();
    test_point_classification();
    test_line_splitting();
    
    return (0);
}