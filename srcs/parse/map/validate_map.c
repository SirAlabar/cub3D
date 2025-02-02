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
