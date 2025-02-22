/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_sidedefs_validate.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/03 17:00:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Validates initial sidedef properties during parsing
 * Only checks texture specifications at this stage
 */
bool    validate_sidedef(t_sidedef *sidedef)
{
    if (!sidedef->upper_texture && !sidedef->middle_texture
        && !sidedef->lower_texture)
    {
        ft_putendl_fd("Warning: Sidedef has no textures", 2);
        return (true);
    }
    

    return (true);
}

/*
 * Validates sector references after all sectors are loaded
 */
bool	validate_sidedef_references(t_doom_map *map)
{
	int	i;

	i = -1;
	while (++i < map->sidedef_count)
	{
		if (map->sidedefs[i].sector < 0 || 
			map->sidedefs[i].sector >= map->sector_count)
		{
			ft_putendl_fd("Error\nInvalid sector reference in sidedef", 2);
			return (false);
		}
	}
	return (true);
}

/*
 * Validates all sidedefs basic properties
 * Called before reference validation
 */
bool	validate_all_sidedefs(t_doom_map *map)
{
	int	i;

	if (map->sidedef_count == 0)
	{
		ft_putendl_fd("Error\nNo sidedefs defined in map", 2);
		return (false);
	}
	i = -1;
	while (++i < map->sidedef_count)
	{
		if (!validate_sidedef(&map->sidedefs[i]))
		{
			ft_printf(RED"Validation failed for sidedef %d\n"DEFAULT, i);
			return (false);
		}
	}
	return (true);
}
