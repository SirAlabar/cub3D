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
 * Skip whitespace and tab characters
 * Returns pointer to first non-whitespace character
 */
static char	*skip_spaces(char *str)
{
	while (str && *str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

/*
 * Initialize linedef array for sector
 * Collects all linedefs that reference this sector
 * Returns false if allocation fails
 */
static bool	init_sector_linedefs(t_sector *sector, t_doom_map *map, int sector_num)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	while (++i < map->linedef_count)
	{
		if (map->linedefs[i].front_sector == sector_num ||
			map->linedefs[i].back_sector == sector_num)
			count++;
	}

	sector->linedef_count = count;
	sector->linedefs = ft_calloc(count, sizeof(int));
	if (!sector->linedefs)
		return (false);

	count = 0;
	i = -1;
	while (++i < map->linedef_count)
	{
		if (map->linedefs[i].front_sector == sector_num ||
			map->linedefs[i].back_sector == sector_num)
			sector->linedefs[count++] = i;
	}
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
	char	*clean;
	bool	success;

	ft_printf("Parsing sector data: '%s'\n", data);
	parts = ft_split(data, ',');
	if (!parts)
		return (false);

	success = false;
	if (parts[0] && parts[1] && parts[2] && parts[3] && parts[4] && !parts[5])
	{
		clean = skip_spaces(parts[0]);
		sector->floor_height = int_to_fixed32(ft_atoi(clean));
		ft_printf("Floor height: %d\n", ft_atoi(clean));

		clean = skip_spaces(parts[1]);
		sector->ceiling_height = int_to_fixed32(ft_atoi(clean));
		ft_printf("Ceiling height: %d\n", ft_atoi(clean));

		clean = skip_spaces(parts[2]);
		sector->light = ft_atoi(clean);
		ft_printf("Light: %d\n", ft_atoi(clean));

		clean = skip_spaces(parts[3]);
		sector->floor_texture = ft_strdup(clean);
		ft_printf("Floor texture: '%s'\n", clean);

		clean = skip_spaces(parts[4]);
		sector->ceiling_texture = ft_strdup(clean);
		ft_printf("Ceiling texture: '%s'\n", clean);

		success = true;
	}
	free_split(parts);
	return (success);
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
	bool		success;

	ft_printf("Parsing sector: '%s'\n", line);
	tokens = ft_split(line, '=');
	if (!tokens || !tokens[0] || !tokens[1])
		return (free_split(tokens), false);

	success = false;
	trimmed = ft_strtrim(tokens[0], " \t\n\r");
	if (get_sector_number(trimmed, &sector_num))
	{
		free(trimmed);
		trimmed = ft_strtrim(tokens[1], " \t\n\r");
		ft_bzero(&new_sector, sizeof(t_sector));
		if (get_sector_data(trimmed, &new_sector))
		{
			map->sectors[sector_num] = new_sector;
			if (!init_sector_linedefs(&map->sectors[sector_num], map, sector_num))
				ft_printf(RED"Failed to initialize sector linedefs\n"DEFAULT);
			else
				success = true;
			if (sector_num >= map->sector_count)
				map->sector_count = sector_num + 1;
		}
	}
	free(trimmed);
	free_split(tokens);
	return (success);
}
