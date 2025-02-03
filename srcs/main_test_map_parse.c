/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_map.c                                         :+:      :+:    :+:   */
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
 * Print a section of vertices with their coordinates
 */
static void	print_vertices(t_doom_map *map)
{
	int	i;

	ft_printf("\nVertices (%d):\n", map->vertex_count);
	i = -1;
	while (++i < map->vertex_count)
		ft_printf("  v%d: (%d,%d)\n", i,
			fixed32_to_int(map->vertices[i].pos.x),
			fixed32_to_int(map->vertices[i].pos.y));
}

/*
 * Print linedefs with their connections and properties
 */
static void	print_linedefs(t_doom_map *map)
{
	int	i;

	ft_printf("\nLinedefs (%d):\n", map->linedef_count);
	i = -1;
	while (++i < map->linedef_count)
	{
		ft_printf("  l%d: v%d->v%d (s%d|s%d) type:%d\n", i,
			map->linedefs[i].vertex1,
			map->linedefs[i].vertex2,
			map->linedefs[i].front_sector,
			map->linedefs[i].back_sector,
			map->linedefs[i].type);
	}
}

/*
 * Print sectors with their properties and textures
 */
static void	print_sectors(t_doom_map *map)
{
	int	i;

	ft_printf("\nSectors (%d):\n", map->sector_count);
	i = -1;
	while (++i < map->sector_count)
	{
		ft_printf("  s%d:\n", i);
		ft_printf("    Floor:   %d\n", fixed32_to_int(map->sectors[i].floor_height));
		ft_printf("    Ceiling: %d\n", fixed32_to_int(map->sectors[i].ceiling_height));
		ft_printf("    Light:   %d\n", map->sectors[i].light);
		ft_printf("    Textures: %s (floor), %s (ceiling)\n",
			map->sectors[i].floor_texture,
			map->sectors[i].ceiling_texture);
	}
}

/*
 * Print all things (player, enemies, items) with positions
 */
static void	print_things(t_doom_map *map)
{
	int		i;
	char	*type_str;

	ft_printf("\nThings (%d):\n", map->thing_count);
	i = -1;
	while (++i < map->thing_count)
	{
		if (map->things[i].type == 1)
			type_str = "Player";
		else if (map->things[i].type == 2)
			type_str = "Enemy";
		else
			type_str = "Item";
		ft_printf("  %s at (%d,%d) angle:%d\n",
			type_str,
			fixed32_to_int(map->things[i].pos.x),
			fixed32_to_int(map->things[i].pos.y),
			fixed32_to_int(map->things[i].angle));
	}
}

/*
 * Print complete map structure in a readable format
 */
static void	print_map_details(t_doom_map *map)
{
	ft_printf("\n=== Map Details ===\n");
	print_vertices(map);
	print_linedefs(map);
	print_sectors(map);
	print_things(map);
	ft_printf("\n=================\n");
}

/*
 * Main function for map parsing and validation
 * Shows detailed information about map structure
 */
int	main(int argc, char **argv)
{
	t_doom_map	map;

	if (!validate_map_extension(argc, argv))
		return (1);
	ft_printf(CYAN"\nLoading map: %s\n"DEFAULT, argv[1]);
	ft_printf(YELLOW"Validating file format...\n"DEFAULT);
	if (!load_map(argc, argv, &map))
	{
		ft_printf(RED"Failed to load map!\n"DEFAULT);
		return (1);
	}
	ft_printf(GREEN"Map loaded successfully!\n"DEFAULT);
	print_map_details(&map);
	cleanup_map(&map);
	return (0);
}


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
