/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_vertices_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:27:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 16:27:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Gets vertex number from identifier
 * Format: vN where N is vertex number
 * Returns false if format is invalid
 */
static bool	get_vertex_number(char *str, int *number)
{
	if (!str || str[0] != 'v')
		return (false);
	*number = ft_atoi(str + 1);
	if (*number < 0 || *number >= MAX_VERTICES)
	{
		ft_putendl_fd("Error\nVertex number out of range", 2);
		return (false);
	}
	return (true);
}

bool validate_vertex_coords(t_fixed_vec32 pos)
{
    int x = fixed32_to_int(pos.x);
    int y = fixed32_to_int(pos.y);
    
    if (x < MAP_COORD_MIN || x > MAP_COORD_MAX ||
        y < MAP_COORD_MIN || y > MAP_COORD_MAX)
    {
        ft_printf("Vertex coordinates out of bounds: (%d,%d)\n", x, y);
        return false;
    }
    return true;
}

/*
 * Parses coordinates string into fixed point values
 * Format: x,y
 * Returns false if format is invalid
 */
 static bool parse_vertex_coords(char *coords, t_fixed_vec32 *pos)
 {
     char    **parts;
     int     input_x, input_y;
     bool    success;
 
     success = false;
     parts = ft_split(coords, ',');
     if (!parts)
         return (false);
 
     if (parts[0] && parts[1] && !parts[2])
     {
         input_x = ft_atoi(parts[0]);
         input_y = ft_atoi(parts[1]);
         pos->x = int_to_fixed32(input_x);
         pos->y = int_to_fixed32(input_y);
         if (input_x >= MAP_COORD_MIN && input_x <= MAP_COORD_MAX &&
             input_y >= MAP_COORD_MIN && input_y <= MAP_COORD_MAX)
         {
             success = true;
         }
     }
     free_split(parts);
     return (success);
 }

/*
 * Parses a vertex definition line
 * Format: vN = x,y
 * Returns false if parsing fails
 */
bool    parse_vertices_section(char *line, t_doom_map *map)
{
    char        **tokens;
    int         vertex_num;
    char        *trimmed;
    bool        success;

    tokens = ft_split(line, '=');
    if (!tokens)
        return (false);
    success = false;
    if (tokens[0] && tokens[1])
    {
        trimmed = ft_strtrim(tokens[0], " \t");
        if (get_vertex_number(trimmed, &vertex_num))
        {
            free(trimmed);
            trimmed = ft_strtrim(tokens[1], " \t");
            if (parse_vertex_coords(trimmed, &map->vertices[vertex_num].pos))
            {
                if (vertex_num >= map->vertex_count)
                    map->vertex_count = vertex_num + 1;
                success = true;
            }
        }
        free(trimmed);
    }
    free_split(tokens);
    return (success);
}
