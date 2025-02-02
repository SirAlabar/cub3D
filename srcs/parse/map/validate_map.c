/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:15:56 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/22 13:57:31 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Validates command line arguments and map file extension
 * Requires exactly two arguments (program name and map file)
 * Prints appropriate error messages if validation fails
 */
bool	validate_map_extension(int argc, char **argv)
{
	if (argc != 2)
		return (printf(ERROR_USAGE), false);
	if (!valid_extension(argv[1]))
		return (printf(ERROR_USAGE ERROR_MAPTYPE), false);
	return (true);
}

/*
 * Checks if filename has .cub extension
 * Returns true only for files ending in .cub
 * Case sensitive comparison
 */
bool	valid_extension(char *map)
{
	const char	*dot;

	dot = ft_strrchr(map, '.');
	if (!dot)
		return (false);
	if (ft_strcmp((char *)dot, ".cub") == 0)
		return (true);
	return (false);
}

/*
 * Opens map file after validating arguments and extension
 * Returns file descriptor or -1 on failure
 * Prints error message if file can't be opened
 */
int	open_map(int argc, char **argv)
{
	int	fd;

	if (!validate_map_extension(argc, argv))
		return (-1);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		ft_putendl_fd("Error\nFailed to open map file", 2);
	return (fd);
}


/*
 * Check basic map requirements
 * - At least one vertex
 * - At least one sector
 * - At least one linedef
 * - Exactly one player
 */
static bool	check_map_basics(t_doom_map *map)
{
	ft_printf("Checking map basics:\n");
	ft_printf("- Vertices: %d\n", map->vertex_count);
	ft_printf("- Linedefs: %d\n", map->linedef_count);
	ft_printf("- Sectors: %d\n", map->sector_count);
	ft_printf("- Things: %d\n", map->thing_count);

	if (map->vertex_count == 0)
	{
		ft_putendl_fd("Error\nNo vertices defined in map", 2);
		return (false);
	}
	if (map->linedef_count == 0)
	{
		ft_putendl_fd("Error\nNo linedefs defined in map", 2);
		return (false);
	}
	if (map->sector_count == 0)
	{
		ft_putendl_fd("Error\nNo sectors defined in map", 2);
		return (false);
	}
	return (true);
}

/*
 * Validates map structure and content
 * Performs sequential checks to identify issues early
 */
bool	validate_map(t_doom_map *map)
{
	ft_printf("Starting map validation...\n");
	if (!check_map_basics(map))
		return (false);

	ft_printf("Validating sectors...\n");
	if (!validate_sectors(map))
	{
		ft_putendl_fd("Error\nSector validation failed", 2);
		return (false);
	}

	ft_printf("Validating textures...\n");
	if (!validate_all_textures(map))
	{
		ft_putendl_fd("Error\nTexture validation failed", 2);
		return (false);
	}

	ft_printf("Validating things...\n");
	if (!validate_things(map))
	{
		ft_putendl_fd("Error\nThing validation failed", 2);
		return (false);
	}

	ft_printf("Map validation complete!\n");
	return (true);
}