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
 * Processes vertex reference
 * Format: vN where N is vertex number
 * Returns false if vertex doesn't exist
 */
static bool	parse_vertex_ref(char *str, int *vertex)
{
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\n\r");
	if (!trimmed || trimmed[0] != 'v')
	{
		free(trimmed);
		return (false);
	}
	*vertex = ft_atoi(trimmed + 1);
	free(trimmed);
	return (true);
}

/*
 * Processes sector reference
 * Format: sN or -1
 * Returns false if format is invalid
 */
static bool	parse_sector_ref(char *str, int *sector)
{
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\n\r");
	if (!trimmed)
		return (false);
	if (trimmed[0] == 's')
		*sector = ft_atoi(trimmed + 1);
	else if (ft_strcmp(trimmed, "-1") == 0)
		*sector = -1;
	else
	{
		free(trimmed);
		return (false);
	}
	free(trimmed);
	return (true);
}

/*
 * Processes linedef data into components
 * Format: vertex1,vertex2,front_sector,back_sector,type
 * Returns false if any component is invalid
 */
static bool	get_linedef_data(char *data, t_linedef *line)
{
    char	**parts;
    char    *trimmed;
    bool	valid;
    int     type;

    parts = ft_split(data, ',');
    if (!parts)
        return (false);
    valid = false;
    if (parts[0] && parts[1] && parts[2] && parts[3] && parts[4] && !parts[5])
    {
        trimmed = ft_strtrim(parts[4], " \t\n\r");
        type = ft_atoi(trimmed);
        free(trimmed);
        if (parse_vertex_ref(parts[0], &line->vertex1)
            && parse_vertex_ref(parts[1], &line->vertex2)
            && parse_sector_ref(parts[2], &line->front_sidedef)
            && parse_sector_ref(parts[3], &line->back_sidedef)
            && type >= 0 && type <= 2)
        {
            line->type = type;
            valid = true;
        }
    }
    free_split(parts);
    return (valid);
}

/*
 * Parses a linedef definition line
 * Format: lN = vertex1,vertex2,front_sector,back_sector,type
 * Returns false if parsing fails
 */
bool	parse_linedefs_section(char *line, t_doom_map *map)
{
	char		**tokens;
	int			linedef_num;
	char		*trimmed;
	t_linedef	new_line;
	bool		success;

	tokens = ft_split(line, '=');
	if (!tokens || !tokens[0] || !tokens[1])
		return (free_split(tokens), false);

	trimmed = ft_strtrim(tokens[0], " \t\n\r");
	if (!get_linedef_number(trimmed, &linedef_num))
	{
		free(trimmed);
		return (free_split(tokens), false);
	}
	free(trimmed);

	trimmed = ft_strtrim(tokens[1], " \t\n\r");
	success = get_linedef_data(trimmed, &new_line);
	free(trimmed);
	free_split(tokens);

	if (success)
	{
		map->linedefs[linedef_num] = new_line;
		if (linedef_num >= map->linedef_count)
			map->linedef_count = linedef_num + 1;
	}
	return (success);
}
