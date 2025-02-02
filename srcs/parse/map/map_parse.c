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
*
 * Checks if a line is empty or contains only whitespace and comments
 * Comments start with '#' and continue to the end of line
 * Returns true if line contains no meaningful data
 *
static bool	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '#' || line[i] == '\0' || line[i] == '\n')
		return (true);
	return (false);
}

*
 * Identifies map section headers in input file
 * Sections: [VERTICES], [LINEDEFS], [SECTORS], [THINGS]
 * Returns section type or NONE if line isn't a header
 *
static t_section	identify_section(char *line)
{
	char	*trimmed;

	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (NONE);
	if (ft_strcmp(trimmed, "[VERTICES]") == 0)
		return (free(trimmed), VERTICES);
	if (ft_strcmp(trimmed, "[LINEDEFS]") == 0)
		return (free(trimmed), LINEDEFS);
	if (ft_strcmp(trimmed, "[SECTORS]") == 0)
		return (free(trimmed), SECTORS);
	if (ft_strcmp(trimmed, "[THINGS]") == 0)
		return (free(trimmed), THINGS);
	free(trimmed);
	return (NONE);
}

*
 * Routes parsing to appropriate section handler
 * Each section has its own specialized parser
 * Returns false if parsing fails
 *
static bool	parse_section(char *line, t_doom_map *map, t_section section)
{
	bool	result;

	result = false;
	if (section == VERTICES)
	{
		ft_printf("Parsing vertex: %s\n", line);
		result = parse_vertices_section(line, map);
	}
	else if (section == LINEDEFS)
	{
		ft_printf("Parsing linedef: %s\n", line);
		result = parse_linedefs_section(line, map);
	}
	else if (section == SECTORS)
	{
		ft_printf("Parsing sector: %s\n", line);
		result = parse_sectors_section(line, map);
	}
	else if (section == THINGS)
	{
		ft_printf("Parsing thing: %s\n", line);
		result = parse_things_section(line, map);
	}
	if (!result)
		ft_printf("Failed to parse: %s\n", line);
	return (result);
}

*
 * Main map parsing function
 * Reads file line by line and routes content to appropriate parser
 * Tracks current section and handles section transitions
 * Returns true if parsing completes successfully
 *
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
			{
				ft_printf("Found section: %s", line);
				current_section = section;
			}
			else if (current_section != NONE)
			{
				if (!parse_section(line, map, current_section))
				{
					free(line);
					return (false);
				}
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	return (true);
}

*
 * Entry point for map loading process
 * Opens file, initializes map structure, triggers parsing
 * Handles cleanup on failure
 * Returns true if map loads successfully
 *
bool	load_map(int argc, char **argv, t_doom_map *map)
{
	int	fd;

	fd = open_map(argc, argv);
	if (fd == -1)
		return (false);
	ft_printf("File opened successfully\n");
	init_map(map);
	if (!parse_map(fd, map))
	{
		ft_putendl_fd("Error\nMap parsing failed", 2);
		cleanup_map(map);
		close(fd);
		return (false);
	}
	close(fd);
	if (!validate_map(map))
	{
		cleanup_map(map);
		return (false);
	}
	return (true);
}
*/
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

#include "map.h"

/*
 * Checks if a line is empty or contains only whitespace and comments
 * Comments start with '#' and continue to the end of line
 * Returns true if line contains no meaningful data
 */
static bool	is_empty_line(char *line)
{
	while (*line)
	{
		if (*line == '#')
			return (true);
		if (!ft_isspace(*line))
			return (false);
		line++;
	}
	return (true);
}

/*
 * Debug function to print section type
 */
static void	print_section_debug(t_section section)
{
	if (section == VERTICES)
		ft_printf(YELLOW"Found VERTICES section\n"DEFAULT);
	else if (section == LINEDEFS)
		ft_printf(YELLOW"Found LINEDEFS section\n"DEFAULT);
	else if (section == SECTORS)
		ft_printf(YELLOW"Found SECTORS section\n"DEFAULT);
	else if (section == THINGS)
		ft_printf(YELLOW"Found THINGS section\n"DEFAULT);
	else if (section == NONE)
		ft_printf(RED"Section not recognized\n"DEFAULT);
}

/*
 * Identifies map section headers in input file
 * Sections: [VERTICES], [LINEDEFS], [SECTORS], [THINGS]
 * Returns section type or NONE if line isn't a header
 */
static t_section	identify_section(char *line)
{
	t_section	section;
	char		*clean_line;

	clean_line = ft_strtrim(line, " \t\n\r");
	if (!clean_line)
		return (NONE);
	ft_printf(CYAN"Checking section: '%s'\n"DEFAULT, clean_line);
	
	section = NONE;
	if (ft_strcmp(clean_line, "[VERTICES]") == 0)
		section = VERTICES;
	else if (ft_strcmp(clean_line, "[LINEDEFS]") == 0)
		section = LINEDEFS;
	else if (ft_strcmp(clean_line, "[SECTORS]") == 0)
		section = SECTORS;
	else if (ft_strcmp(clean_line, "[THINGS]") == 0)
		section = THINGS;

	free(clean_line);
	print_section_debug(section);
	return (section);
}

/*
 * Routes parsing to appropriate section handler
 * Each section has its own specialized parser
 * Returns false if parsing fails
 */
static bool	parse_section(char *line, t_doom_map *map, t_section section)
{
	char	*clean_line;
	bool	result;

	clean_line = ft_strtrim(line, " \t\n\r");
	if (!clean_line)
		return (false);

	ft_printf(CYAN"Parsing line: '%s' in section ", clean_line);
	print_section_debug(section);

	result = false;
	if (section == VERTICES)
		result = parse_vertices_section(clean_line, map);
	else if (section == LINEDEFS)
		result = parse_linedefs_section(clean_line, map);
	else if (section == SECTORS)
		result = parse_sectors_section(clean_line, map);
	else if (section == THINGS)
		result = parse_things_section(clean_line, map);

	if (!result)
		ft_printf(RED"Failed to parse line: '%s'\n"DEFAULT, clean_line);
	free(clean_line);
	return (result);
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
	while ((line = get_next_line(fd)))
	{
		ft_printf(BLUE"Read line: '%s'"DEFAULT, line);
		if (!is_empty_line(line))
		{
			section = identify_section(line);
			if (section != NONE)
				current_section = section;
			else if (current_section != NONE)
			{
				if (!parse_section(line, map, current_section))
				{
					free(line);
					return (false);
				}
			}
		}
		free(line);
	}
	return (true);
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
	ft_printf(GREEN"File opened successfully\n"DEFAULT);
	init_map(map);
	if (!parse_map(fd, map))
	{
		ft_putendl_fd("Error\nMap parsing failed", 2);
		cleanup_map(map);
		close(fd);
		return (false);
	}
	close(fd);
	if (!validate_map(map))
	{
		cleanup_map(map);
		return (false);
	}
	return (true);
}
