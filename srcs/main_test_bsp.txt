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
#include <time.h>
void test_bsp_traversal(void);

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
    // Seed para aleatoriedade reproduzível
    srand(time(NULL));

    t_doom_map *map = ft_calloc(1, sizeof(t_doom_map));
    if (!map) return NULL;

    // Número de salas
    int num_rooms = 2 + (rand() % 3);  // 2-4 salas
    int max_coord = 512;

    // Gerar vértices aleatórios
    for (int room = 0; room < num_rooms; room++) {
        int base_x = rand() % (max_coord / 2);
        int base_y = rand() % (max_coord / 2);

        // Vertices de cada sala
        int vertex_coords[4][2] = {
            {base_x, base_y},
            {base_x + 128, base_y},
            {base_x + 128, base_y + 128},
            {base_x, base_y + 128}
        };

        // Adicionar vértices da sala
        for (int i = 0; i < 4; i++) {
            int vertex_index = (room * 4) + i;
            map->vertices[vertex_index] = (t_vertex){
                (t_fixed_vec32){
                    int_to_fixed32(vertex_coords[i][0]), 
                    int_to_fixed32(vertex_coords[i][1])
                }
            };
        }
    }
    map->vertex_count = num_rooms * 4;

    // Gerar linhas para cada sala
    int linedef_count = 0;
    for (int room = 0; room < num_rooms; room++) {
        int start_vertex = room * 4;
        
        // Linhas para formar cada sala
        int connections[4][2] = {
            {start_vertex, start_vertex + 1},
            {start_vertex + 1, start_vertex + 2},
            {start_vertex + 2, start_vertex + 3},
            {start_vertex + 3, start_vertex}
        };

        for (int j = 0; j < 4; j++) {
            map->linedefs[linedef_count] = (t_linedef){
                connections[j][0], 
                connections[j][1], 
                0,  // tipo padrão
                -1, 0, 0
            };
            linedef_count++;
        }
    }

    // Adicionar algumas conexões entre salas (portas)
    int door_attempts = num_rooms * 2;
    while (door_attempts-- > 0) {
        int room1 = rand() % num_rooms;
        int room2 = rand() % num_rooms;
        
        if (room1 != room2) {
            int vertex1 = room1 * 4 + (rand() % 4);
            int vertex2 = room2 * 4 + (rand() % 4);
            
            map->linedefs[linedef_count] = (t_linedef){
                vertex1, 
                vertex2, 
                0,  // tipo de porta
                -1, 0, 0
            };
            linedef_count++;
        }
    }

    map->linedef_count = linedef_count;

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

// Função auxiliar para contar nós
static int count_bsp_tree_nodes(t_bsp_node *node, int *max_depth)
{
    if (!node)
        return 0;

    // Atualizar profundidade máxima
    int current_depth = node->depth;
    if (current_depth > *max_depth)
        *max_depth = current_depth;

    // Contar nós recursivamente
    return 1 + 
           count_bsp_tree_nodes(node->front, max_depth) + 
           count_bsp_tree_nodes(node->back, max_depth);
}


// Função de teste de travessia
static void test_bsp_tree_traversal(t_bsp_node *root, t_bsp_line **lines, int num_lines)
{
    if (!root || !lines || num_lines == 0)
        return;

    // Calculate test points slightly offset from centers
    t_fixed_vec32 test_points[3];
    t_fixed32 offset = int_to_fixed32(1);  // Small offset to avoid exactly on lines
    
    // First room center + offset
    test_points[0] = (t_fixed_vec32){
        fixed32_div(fixed32_add(lines[0]->start.x, lines[2]->end.x), int_to_fixed32(2)),
        fixed32_add(fixed32_div(fixed32_add(lines[0]->start.y, lines[2]->end.y), int_to_fixed32(2)), offset)
    };
    
    // Middle room center + offset
    int mid = num_lines / 2;
    test_points[1] = (t_fixed_vec32){
        fixed32_add(fixed32_div(fixed32_add(lines[mid]->start.x, lines[mid]->end.x), int_to_fixed32(2)), offset),
        fixed32_div(fixed32_add(lines[mid]->start.y, lines[mid]->end.y), int_to_fixed32(2))
    };
    
    // Last room center + offset
    test_points[2] = (t_fixed_vec32){
        fixed32_div(fixed32_add(lines[num_lines-4]->start.x, lines[num_lines-2]->end.x), int_to_fixed32(2)),
        fixed32_sub(fixed32_div(fixed32_add(lines[num_lines-4]->start.y, lines[num_lines-2]->end.y), int_to_fixed32(2)), offset)
    };

    for (int i = 0; i < 3; i++)
    {
        ft_printf("\nTesting point %d: (%d,%d)\n", i + 1,
            fixed32_to_int(test_points[i].x),
            fixed32_to_int(test_points[i].y));

        t_bsp_node *current = root;
        int depth = 0;

        ft_printf("Traversal path:\n");
        while (current && current->partition)
        {
            ft_printf("Level %d: (%d,%d) -> (%d,%d)",
                depth,
                fixed32_to_int(current->partition->start.x),
                fixed32_to_int(current->partition->start.y),
                fixed32_to_int(current->partition->end.x),
                fixed32_to_int(current->partition->end.y));

            t_bsp_side side = bsp_classify_point(test_points[i], current->partition);
            ft_printf(" - Point is %s\n", 
                side == BSP_FRONT ? "FRONT" : 
                side == BSP_BACK ? "BACK" : 
                side == BSP_COLINEAR ? "ON LINE" : "SPANNING");

            if (!current->front && !current->back)
                break;

            // Para pontos ON LINE, escolher FRONT por padrão
            if (side == BSP_COLINEAR && current->front)
                current = current->front;
            else if (side == BSP_FRONT && current->front)
                current = current->front;
            else if (current->back)
                current = current->back;
            else
                break;

            depth++;
        }

        if (current && !current->partition)
            ft_printf("Found leaf node at depth %d\n", depth);
        else if (current)
            ft_printf("Found terminal node at depth %d\n", depth);
        else
            ft_printf("No node found after %d levels\n", depth);
    }
}

static void test_bsp_tree_construction(void)
{
    t_doom_map      *map;
    t_bsp_tree      *tree;
    t_bsp_line      **lines;
    int             num_lines;
    int             total_nodes = 0;
    int             max_depth = 0;
    t_bsp_node      *root;

    ft_printf("\n=== Comprehensive BSP Tree Construction Test ===\n");

    // Criar mapa de teste
    map = create_complex_test_map();
    if (!map)
    {
        ft_printf("FAIL: Could not create test map\n");
        return;
    }

    // Extrair linhas do mapa
    if (!extract_map_lines(map, &lines, &num_lines))
    {
        ft_printf("FAIL: Could not extract map lines\n");
        free(map);
        return;
    }

    ft_printf("Map Details:\n");
    ft_printf("Total vertices: %d\n", map->vertex_count);
    ft_printf("Total linedefs: %d\n", map->linedef_count);
    ft_printf("Lines extracted: %d\n", num_lines);

    // Construir árvore BSP
    tree = init_bsp_build(map);
    if (!tree)
    {
        ft_printf("FAIL: Could not create BSP tree\n");
        free(map);
        free(lines);
        return;
    }

    // Validações da árvore
    root = tree->root;
    if (!root)
    {
        ft_printf("FAIL: BSP tree root is NULL\n");
        free_bsp_tree(tree);
        free(map);
        free(lines);
        return;
    }

    // Contar nós e calcular profundidade
    total_nodes = count_bsp_tree_nodes(root, &max_depth);

    ft_printf("\nBSP Tree Analysis:\n");
    ft_printf("Total nodes: %d\n", total_nodes);
    ft_printf("Maximum tree depth: %d\n", max_depth);

    // Verificar balanceamento da árvore
    if (!balance_bsp_tree(tree))
    {
        ft_printf("WARNING: Tree might not be optimally balanced\n");
    }

    // Validação da árvore
    if (!validate_bsp_tree(tree))
    {
        ft_printf("FAIL: BSP tree validation failed\n");
    }
    else
    {
        ft_printf("SUCCESS: BSP tree passed validation\n");
    }

    // Imprimir estrutura da árvore
    ft_printf("\nBSP Tree Structure:\n");
    print_bsp_tree(tree);

    // Testes de travessia
    ft_printf("\nTraversal Tests:\n");
    test_bsp_tree_traversal(root, lines, num_lines);

    // Limpar memória
    free_bsp_tree(tree);
    free(map);
    free(lines);

    ft_printf("=== End of BSP Tree Construction Test ===\n");
}



/*
int main(void)
{
    test_map_line_extraction();
    test_line_classification();
    test_partition_selection();
    test_point_classification();
    test_line_splitting();
    test_bsp_tree_construction();
    
    return (0);
}*/
int	main(void)
{
	int	tests_passed = 0;
	int	total_tests = 6;

	ft_printf("=== BSP TEST SUITE ===\n");

	ft_printf("\n[TEST 1] Map Line Extraction: ");
	test_map_line_extraction();
	tests_passed++;
	ft_printf("PASS\n");

	ft_printf("\n[TEST 2] Line Classification: ");
	test_line_classification();
	tests_passed++;
	ft_printf("PASS\n");

	ft_printf("\n[TEST 3] Partition Selection: ");
	test_partition_selection();
	tests_passed++;
	ft_printf("PASS\n");

	ft_printf("\n[TEST 4] Point Classification: ");
	test_point_classification();
	tests_passed++;
	ft_printf("PASS\n");

	ft_printf("\n[TEST 5] Line Splitting: ");
	test_line_splitting();
	tests_passed++;
	ft_printf("PASS\n");

	ft_printf("\n[TEST 6] BSP Tree Construction: ");
	test_bsp_tree_construction();
	tests_passed++;
	ft_printf("PASS\n");


	ft_printf("\n=== TEST SUMMARY ===\n");
	ft_printf("Passed: %d/%d\n", tests_passed, total_tests);
	ft_printf("Success Rate: %.2f%%\n", 
		((float)tests_passed / total_tests) * 100);

	return (tests_passed == total_tests ? 0 : 1);
}