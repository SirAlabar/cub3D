/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:15:56 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/12 14:14:09 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	validate_map_extension(int argc, char **argv)
{
	if (argc != 2)
		return (printf(ERROR_USAGE), false);
	if (!valid_extension(argv[1]))
		return (printf(ERROR_USAGE ERROR_MAPTYPE), false);
	return (true);
}

bool	valid_extension(char *map)
{
	const char	*dot = ft_strrchr(map, '.');

	if (!dot)
		return (false);
	if (ft_strcmp((char *)dot, ".cub") == 0)
		return (true);
	return (false);
}

bool	validate_texture_paths(t_game *game)
{
	int fd;

	if (!game->north.path || !game->south.path || !game->east.path
		|| !game->west.path)
	{
		ft_printf("Error\nMissing texture path(s)\n");
		return (false);
	}
	fd = open(game->north.path, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error\nCannot open north texture: %s\n", game->north.path);
		return (false);
	}
	close(fd);
	fd = open(game->south.path, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error\nCannot open south texture: %s\n", game->south.path);
		return (false);
	}
	close(fd);
	fd = open(game->east.path, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error\nCannot open east texture: %s\n", game->east.path);
		return (false);
	}
	close(fd);
	fd = open(game->west.path, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error\nCannot open west texture: %s\n", game->west.path);
		return (false);
	}
	close(fd);
	return (true);
}
