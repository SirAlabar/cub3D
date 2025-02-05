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
 * Print sidedefs with their textures and offsets
 */
static void	print_sidedefs(t_doom_map *map)
{
	int	i;

	ft_printf("\nSidedefs (%d):\n", map->sidedef_count);
	i = -1;
	while (++i < map->sidedef_count)
	{
		ft_printf("  sd%d:\n", i);
		ft_printf("    Offset: (%d,%d)\n",
			fixed32_to_int(map->sidedefs[i].x_offset),
			fixed32_to_int(map->sidedefs[i].y_offset));
		ft_printf("    Upper:  %s\n",
			map->sidedefs[i].upper_texture ?
			map->sidedefs[i].upper_texture : "none");
		ft_printf("    Middle: %s\n",
			map->sidedefs[i].middle_texture ?
			map->sidedefs[i].middle_texture : "none");
		ft_printf("    Lower:  %s\n",
			map->sidedefs[i].lower_texture ?
			map->sidedefs[i].lower_texture : "none");
		ft_printf("    Sector: s%d\n", map->sidedefs[i].sector);
	}
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
		ft_printf("  l%d: v%d->v%d ", i,
			map->linedefs[i].vertex1,
			map->linedefs[i].vertex2);
		ft_printf("(sd%d|", map->linedefs[i].front_sidedef);
		if (map->linedefs[i].back_sidedef == -1)
			ft_printf("-) ");
		else
			ft_printf("sd%d) ", map->linedefs[i].back_sidedef);
		ft_printf("type:%d\n", map->linedefs[i].type);
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
	print_sidedefs(map); 
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

