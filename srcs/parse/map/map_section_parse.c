/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_section_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 19:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Copies texture names to sector structure
 * Validates texture name length
 * Returns false if names exceed maximum length
 */
static bool	set_textures(t_sector *sector, char *floor, char *ceiling)
{
	if (ft_strlen(floor) >= MAX_TEXTURE_NAME
		|| ft_strlen(ceiling) >= MAX_TEXTURE_NAME)
		return (false);
	ft_strlcpy(sector->floor_texture, floor, MAX_TEXTURE_NAME);
	ft_strlcpy(sector->ceiling_texture, ceiling, MAX_TEXTURE_NAME);
	return (true);
}

/*
 * Processes sector data fields into sector structure
 * Format: floor_height,ceiling_height,light,floor_texture,ceiling_texture
 * Returns false if any field is invalid
 */
static bool	get_sector_data(char *data, t_sector *sector)
{
	char	**parts;
	bool	success;
	int		floor;
	int		ceiling;
	int		light;

	success = false;
	parts = ft_split(data, ',');
	if (!parts)
		return (false);
	if (parts[0] && parts[1] && parts[2] && parts[3] && parts[4] && !parts[5])
	{
		floor = ft_atoi(ft_strtrim(parts[0], " \t"));
		ceiling = ft_atoi(ft_strtrim(parts[1], " \t"));
		light = ft_atoi(ft_strtrim(parts[2], " \t"));
		if (validate_heights(sector, floor, ceiling) && validate_light(sector,
				light) && set_textures(sector, ft_strtrim(parts[3], " \t"),
				ft_strtrim(parts[4], " \t")))
			success = true;
	}
	return (free_split(parts), success);
}

/*
 * Parses a sector definition line and stores it in the map
 * Format: sN = floor_height,ceiling_height,light,floor_texture,ceiling_texture
 * Updates sector count if new index is higher than current count
 */
bool	parse_sectors_section(char *line, t_doom_map *map)
{
	char		**tokens;
	int			sector_num;
	char		*trimmed;
	t_sector	new_sector;

	tokens = ft_split(line, '=');
	if (!tokens)
		return (false);
	if (!tokens[0] || !tokens[1])
		return (free_split(tokens), false);
	trimmed = ft_strtrim(tokens[0], " \t");
	if (!get_sector_number(trimmed, &sector_num))
		return (free(trimmed), free_split(tokens), false);
	free(trimmed);
	trimmed = ft_strtrim(tokens[1], " \t");
	if (!get_sector_data(trimmed, &new_sector))
		return (free(trimmed), free_split(tokens), false);
	free(trimmed);
	free_split(tokens);
	map->sectors[sector_num] = new_sector;
	if (sector_num >= map->sector_count)
		map->sector_count = sector_num + 1;
	return (true);
}
