/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 17:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Checks if a line is empty or contains only whitespace and comments
 * Comments start with '#' and continue to the end of line
 * Returns true if line contains no meaningful data
 */
static bool	is_empty_line(char *line)
{
	while (*line && *line != '\n')
	{
		if (!ft_isspace(*line) && *line != '#')
			return (false);
		line++;
	}
	return (true);
}

/*
 * Identifies map section headers in input file
 * Sections: [VERTICES], [LINEDEFS], [SECTORS], [THINGS]
 * Returns section type or NONE if line isn't a header
 */
static t_section	identify_section(char *line)
{
	if (ft_strncmp(line, "[VERTICES]", 10) == 0)
		return (VERTICES);
	if (ft_strncmp(line, "[LINEDEFS]", 10) == 0)
		return (LINEDEFS);
	if (ft_strncmp(line, "[SECTORS]", 9) == 0)
		return (SECTORS);
	if (ft_strncmp(line, "[THINGS]", 8) == 0)
		return (THINGS);
	return (NONE);
}

/*
 * Main map parsing function
 * Reads file line by line and routes content to appropriate parser
 * Tracks current section and handles section transitions
 * Returns true if parsing completes successfully
 */
bool	parse_map(int fd, t_doom_map *map)
{
	char		*line;
	t_section	current_section;
	t_section	section;

	current_section = NONE;
	line = get_next_line(fd);
	while (line)
	{
		if (!is_empty_line(line))
		{
			section = identify_section(line);
			if (section != NONE)
				current_section = section;
			else if (current_section != NONE)
			{
				if (!parse_section(line, map, current_section))
					return (free(line), false);
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	return (true);
}

/*
 * Routes parsing to appropriate section handler
 * Each section has its own specialized parser
 * Returns false if parsing fails
 */
static bool	parse_section(char *line, t_doom_map *map, t_section section)
{
	if (section == VERTICES)
		return (parse_vertices_section(line, map));
	if (section == LINEDEFS)
		return (parse_linedefs_section(line, map));
	if (section == SECTORS)
		return (parse_sectors_section(line, map));
	if (section == THINGS)
		return (parse_things_section(line, map));
	return (false);
}

/*
 * Entry point for map loading process
 * Opens file, initializes map structure, triggers parsing
 * Handles cleanup on failure
 * Returns true if map loads successfully
 */
bool	load_map(int argc, char **argv, t_doom_map *map)
{
	int	fd;

	fd = open_map(argc, argv);
	if (fd == -1)
		return (false);
	init_map(map);
	if (!parse_map(fd, map))
	{
		cleanup_map(map);
		close(fd);
		return (false);
	}
	close(fd);
	return (true);
}
