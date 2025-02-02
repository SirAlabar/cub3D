/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_section_parse .c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 18:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Processes vertex reference in linedef definition
 * Format: vN where N is a vertex index (e.g., "v0", "v1")
 * Stores the vertex index in the vertex parameter
 */
static bool	parse_vertex_ref(char *str, int *vertex)
{
	if (!str || str[0] != 'v')
		return (false);
	*vertex = ft_atoi(str + 1);
	return (true);
}

/*
 * Processes sector reference in linedef definition
 * Format: sN where N is a sector index, or -1 for no sector
 * Used for both front and back sector references
 */
static bool	parse_sector_ref(char *str, int *sector)
{
	if (!str)
		return (false);
	if (str[0] == 's')
		*sector = ft_atoi(str + 1);
	else if (str[0] == '-')
		*sector = -1;
	else
		return (false);
	return (true);
}

/*
 * Validates linedef data against map constraints
 * Checks:
 * - Vertex indices within valid range
 * - Sector references exist
 * - Line type is valid (0:wall, 1:door, 2:transparent)
 */
static bool	validate_linedef_data(t_linedef *line, t_doom_map *map)
{
	if (line->vertex1 >= map->vertex_count
		|| line->vertex2 >= map->vertex_count)
		return (false);
	if (line->front_sector >= map->sector_count)
		return (false);
	if (line->back_sector >= 0 && line->back_sector >= map->sector_count)
		return (false);
	if (line->type < 0 || line->type > 2)
		return (false);
	return (true);
}

/*
 * Processes linedef data string into linedef structure
 * Format: vertex1,vertex2,front_sector,back_sector,type
 * Returns false if any field is invalid
 */
static bool	get_linedef_data(char *data, t_linedef *line, t_doom_map *map)
{
	char	**parts;
	bool	success;

	success = false;
	parts = ft_split(data, ',');
	if (!parts)
		return (false);
	if (parts[0] && parts[1] && parts[2] && parts[3] && parts[4] && !parts[5])
	{
		if (parse_vertex_ref(ft_strtrim(parts[0], " \t"), &line->vertex1)
			&& parse_vertex_ref(ft_strtrim(parts[1], " \t"), &line->vertex2)
			&& parse_sector_ref(ft_strtrim(parts[2], " \t"),
				&line->front_sector) && parse_sector_ref(ft_strtrim(parts[3],
					" \t"), &line->back_sector))
		{
			line->type = ft_atoi(parts[4]);
			success = validate_linedef_data(line, map);
		}
	}
	return (free_split(parts), success);
}

/*
 * Parses a linedef definition line and stores it in the map
 * Format: lN = vertex1,vertex2,front_sector,back_sector,type
 * Updates linedef count if new index is higher than current count
 */
bool	parse_linedefs_section(char *line, t_doom_map *map)
{
	char		**tokens;
	int			linedef_num;
	char		*trimmed;
	t_linedef	new_line;

	tokens = ft_split(line, '=');
	if (!tokens)
		return (false);
	if (!tokens[0] || !tokens[1])
		return (free_split(tokens), false);
	trimmed = ft_strtrim(tokens[0], " \t");
	if (!get_linedef_number(trimmed, &linedef_num))
		return (free(trimmed), free_split(tokens), false);
	free(trimmed);
	trimmed = ft_strtrim(tokens[1], " \t");
	if (!get_linedef_data(trimmed, &new_line, map))
		return (free(trimmed), free_split(tokens), false);
	free(trimmed);
	free_split(tokens);
	map->linedefs[linedef_num] = new_line;
	if (linedef_num >= map->linedef_count)
		map->linedef_count = linedef_num + 1;
	return (true);
}
