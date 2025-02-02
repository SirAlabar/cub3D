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
 * Returns false if type is invalid
 */
static bool	validate_thing_type(t_doom_map *map, int type)
{
	int	i;

	ft_printf("Validating thing type: %d\n", type);
	if (type < 1 || type > 3)
	{
		ft_printf(RED"Invalid thing type: %d\n"DEFAULT, type);
		return (false);
	}
	if (type == 1)
	{
		i = -1;
		while (++i < map->thing_count)
		{
			if (map->things[i].type == 1)
			{
				ft_printf(RED"Multiple player things found\n"DEFAULT);
				return (false);
			}
		}
	}
	return (true);
}

/*
 * Processes thing data into component values
 * Format: type,x,y,angle
 * Returns false if any component is invalid
 */
static bool	get_thing_data(char *line, t_thing *thing, t_doom_map *map)
{
	char	**parts;
	bool	success;
	int		type;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \t\n\r");
	parts = ft_split(trimmed, ',');
	free(trimmed);
	if (!parts)
		return (false);

	success = false;
	if (parts[0] && parts[1] && parts[2] && parts[3] && !parts[4])
	{
		type = ft_atoi(parts[0]);
		if (validate_thing_type(map, type))
		{
			thing->type = type;
			thing->pos.x = int_to_fixed32(ft_atoi(parts[1]));
			thing->pos.y = int_to_fixed32(ft_atoi(parts[2]));
			thing->angle = int_to_fixed32(ft_atoi(parts[3]));
			success = true;
		}
	}
	else
		ft_printf(RED"Invalid thing data format\n"DEFAULT);

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

	ft_printf("Parsing thing: '%s'\n", line);
	if (map->thing_count >= MAX_THINGS)
	{
		ft_printf(RED"Maximum number of things reached\n"DEFAULT);
		return (false);
	}

	if (!get_thing_data(line, &new_thing, map))
	{
		ft_printf(RED"Failed to parse thing data\n"DEFAULT);
		return (false);
	}

	ft_printf(GREEN"Adding thing: type=%d pos=(%d,%d) angle=%d\n"DEFAULT,
		new_thing.type,
		fixed32_to_int(new_thing.pos.x),
		fixed32_to_int(new_thing.pos.y),
		fixed32_to_int(new_thing.angle));

	map->things[map->thing_count++] = new_thing;
	return (true);
}
