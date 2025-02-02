/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_things_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 20:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Validates thing type (1:player, 2:enemy, 3:item)
 * Ensures only one player exists in the map
 * Returns false if type is invalid or multiple players found
 */
static bool	validate_thing_type(t_doom_map *map, int type)
{
	int	i;

	if (type < 1 || type > 3)
		return (false);
	if (type == 1)
	{
		i = 0;
		while (i < map->thing_count)
		{
			if (map->things[i].type == 1)
				return (false);
			i++;
		}
	}
	return (true);
}

static bool	build_position_str(char *s1, char *s2, char **result)
{
	char	*tmp;

	tmp = ft_strjoin(s1, ",");
	if (!tmp)
		return (false);
	*result = ft_strjoin(tmp, s2);
	free(tmp);
	if (!*result)
		return (false);
	return (true);
}

/*
 * Converts position string to fixed point coordinates
 * Format: "x,y"
 * Returns false if conversion fails
 */
static bool	parse_position(char *pos_str, t_fixed_vec32 *pos)
{
	char	**coords;
	char	*trim1;
	char	*trim2;
	bool	success;

	success = false;
	coords = ft_split(pos_str, ',');
	if (!coords)
		return (false);
	if (coords[0] && coords[1] && !coords[2])
	{
		trim1 = ft_strtrim(coords[0], " \t");
		trim2 = ft_strtrim(coords[1], " \t");
		if (trim1 && trim2)
		{
			pos->x = int_to_fixed32(ft_atoi(trim1));
			pos->y = int_to_fixed32(ft_atoi(trim2));
			success = true;
		}
		free(trim1);
		free(trim2);
	}
	free_split(coords);
	return (success);
}

/*
 * Validates and converts angle to fixed point
 * Angle must be between 0 and 359 degrees
 * 0 = east, 90 = north
 */
static bool	parse_angle(char *angle_str, t_fixed32 *angle)
{
	int	degrees;

	degrees = ft_atoi(angle_str);
	if (degrees < 0 || degrees > 359)
		return (false);
	*angle = int_to_fixed32(degrees);
	return (true);
}

/*
 * Processes thing data into component values
 * Format: type,x,y,angle
 * Returns false if any component is invalid
 */
static bool	get_thing_data(char *data, t_thing *thing, t_doom_map *map)
{
	char	**parts;
	char	*pos_str;
	char	*trim;
	bool	success;

	parts = ft_split(data, ',');
	if (!parts || !parts[0] || !parts[1] || !parts[2] || !parts[3] || parts[4])
		return (free_split(parts), false);
	trim = ft_strtrim(parts[0], " \t");
	if (!trim || !build_position_str(parts[1], parts[2], &pos_str))
		return (free(trim), free_split(parts), false);
	thing->type = ft_atoi(trim);
	free(trim);
	trim = ft_strtrim(parts[3], " \t");
	success = (validate_thing_type(map, thing->type)
			&& parse_position(pos_str, &thing->pos)
			&& parse_angle(trim, &thing->angle));
	free(trim);
	free(pos_str);
	free_split(parts);
	return (success);
}

/*
 * Parses a thing definition and adds it to the map
 * Format: type,x,y,angle
 * Returns false if parsing fails
 */
bool	parse_things_section(char *line, t_doom_map *map)
{
	t_thing	new_thing;

	if (map->thing_count >= MAX_THINGS)
		return (false);
	if (!get_thing_data(line, &new_thing, map))
		return (false);
	map->things[map->thing_count] = new_thing;
	map->thing_count++;
	return (true);
}