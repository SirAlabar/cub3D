/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_texture_validate.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 23:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Checks if a texture file exists and is readable
 * Attempts to open the file without actually loading the texture
 * Returns false if file cannot be accessed or is not an XPM
 */
bool	texture_exists(char *texture_path)
{
	int		fd;
	char	*extension;

	if (!texture_path)
		return (false);
	extension = ft_strrchr(texture_path, '.');
	if (!extension || ft_strcmp(extension, ".xpm") != 0)
	{
		ft_putendl_fd("Error\nTexture must be an XPM file: ", 2);
		ft_putendl_fd(texture_path, 2);
		return (false);
	}
	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		ft_putendl_fd("Error\nCannot access texture file: ", 2);
		ft_putendl_fd(texture_path, 2);
		return (false);
	}
	close(fd);
	return (true);
}

/*
 * Checks if all required textures for the map exist
 * Validates both sector textures and wall textures
 * Returns false if any required texture is missing or invalid
 */
bool	validate_all_textures(t_doom_map *map)
{
	int	i;

	i = 0;
	while (i < map->sector_count)
	{
		if (!texture_exists(map->sectors[i].floor_texture)
			|| !texture_exists(map->sectors[i].ceiling_texture))
			return (false);
		i++;
	}
	return (true);
}
