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
