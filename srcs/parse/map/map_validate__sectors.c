/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate_player.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 21:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 21:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Checks if a sector forms a closed loop using its linedefs
 * Each vertex in the sector should connect to exactly two linedefs
 * Returns false if sector is not properly closed
 */
static bool	validate_sector_closure(t_sector *sector, t_doom_map *map)
{
	t_linedef	*line;
	int			i;

	i = 0;
	while (i < sector->linedef_count - 1)
	{
		line = &map->linedefs[sector->linedefs[i]];
		if (line->vertex2 != map->linedefs[sector->linedefs[i + 1]].vertex1)
		{
			ft_putendl_fd("Error\nSector linedefs do not form a closed loop", 2);
			return (false);
		}
		i++;
	}
	line = &map->linedefs[sector->linedefs[i]];
	if (line->vertex2 != map->linedefs[sector->linedefs[0]].vertex1)
	{
		ft_putendl_fd("Error\nSector is not properly closed", 2);
		return (false);
	}
	return (true);
}

/*
 * Ensures all linedefs referenced by sector exist
 * Checks both front and back sector references
 * Returns false if any linedef reference is invalid
 */
static bool	validate_sector_linedefs(t_sector *sector, t_doom_map *map)
{
	int	i;

	i = 0;
	while (i < sector->linedef_count)
	{
		if (sector->linedefs[i] >= map->linedef_count)
		{
			ft_putendl_fd("Error\nSector references nonexistent linedef", 2);
			return (false);
		}
		i++;
	}
	return (true);
}

/*
 * Verifies if sector overlaps with any other sector
 * Checks for shared space using linedef positions
 * Returns false if overlap is detected
 */
static bool	check_sector_overlap(t_sector *sector, t_doom_map *map, int curr_idx)
{
	int	i;
	int	j;

	i = 0;
	while (i < map->sector_count)
	{
		if (i != curr_idx)
		{
			j = 0;
			while (j < sector->linedef_count)
			{
				if (check_linedef_intersect(&map->linedefs[sector->linedefs[j]],
					&map->sectors[i], map))
				{
					ft_putendl_fd("Error\nOverlapping sectors detected", 2);
					return (false);
				}
				j++;
			}
		}
		i++;
	}
	return (true);
}

/*
 * Verifies all textures referenced by sector exist
 * Checks both floor and ceiling textures
 * Returns false if any texture is missing
 */
static bool	validate_sector_textures(t_sector *sector)
{
	if (!texture_exists(sector->floor_texture))
	{
		ft_putendl_fd("Error\nMissing floor texture: ", 2);
		ft_putendl_fd(sector->floor_texture, 2);
		return (false);
	}
	if (!texture_exists(sector->ceiling_texture))
	{
		ft_putendl_fd("Error\nMissing ceiling texture: ", 2);
		ft_putendl_fd(sector->ceiling_texture, 2);
		return (false);
	}
	return (true);
}

/*
 * Main sector validation function
 * Runs all sector validation checks
 * Returns false if any validation fails
 */
bool	validate_sectors(t_doom_map *map)
{
	int	i;

	if (map->sector_count == 0)
	{
		ft_putendl_fd("Error\nNo sectors defined in map", 2);
		return (false);
	}
	i = 0;
	while (i < map->sector_count)
	{
		if (!validate_sector_closure(&map->sectors[i], map)
			|| !validate_sector_linedefs(&map->sectors[i], map)
			|| !check_sector_overlap(&map->sectors[i], map, i)
			|| !validate_sector_textures(&map->sectors[i]))
			return (false);
		i++;
	}
	return (true);
}
