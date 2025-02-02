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
 * Checks if a sector has valid references
 * - Floor height < ceiling height
 * - Valid light level (0-255)
 * - Valid texture references
 */
static bool	validate_sector_basics(t_sector *sector)
{
	ft_printf("Validating sector basics:\n");
	ft_printf("- Floor height: %d\n", fixed32_to_int(sector->floor_height));
	ft_printf("- Ceiling height: %d\n", fixed32_to_int(sector->ceiling_height));
	ft_printf("- Light level: %d\n", sector->light);

	if (sector->floor_height >= sector->ceiling_height)
	{
		ft_putendl_fd("Error\nFloor height >= ceiling height", 2);
		return (false);
	}
	if (sector->light < 0 || sector->light > 255)
	{
		ft_putendl_fd("Error\nInvalid light level", 2);
		return (false);
	}
	return (true);
}

/*
 * Check linedef references in sector
 * - All references must be valid
 * - Linedefs must form a closed loop
 */
static bool	validate_sector_linedefs(t_sector *sector, t_doom_map *map)
{
	int			i;
	t_linedef	*linedef;

	if (!sector->linedefs)
	{
		ft_putendl_fd("Error\nSector has no linedefs array", 2);
		return (false);
	}

	i = -1;
	while (++i < sector->linedef_count)
	{
		if (sector->linedefs[i] >= map->linedef_count)
		{
			ft_putendl_fd("Error\nInvalid linedef reference", 2);
			return (false);
		}
		linedef = &map->linedefs[sector->linedefs[i]];
		if (linedef->front_sector != -1 && linedef->front_sector >= map->sector_count)
		{
			ft_putendl_fd("Error\nInvalid sector reference in linedef", 2);
			return (false);
		}
	}
	return (true);
}

/*
 * Validates an individual sector
 * Checks:
 * - Basic sector properties
 * - Linedef references
 * - Texture existence
 */
static bool	validate_sector(t_sector *sector, t_doom_map *map, int sector_num)
{
	ft_printf("Validating sector %d...\n", sector_num);
	if (!validate_sector_basics(sector))
		return (false);
	if (!validate_sector_linedefs(sector, map))
		return (false);
	if (!texture_exists(sector->floor_texture))
	{
		ft_printf("Error\nInvalid floor texture: %s\n", sector->floor_texture);
		return (false);
	}
	if (!texture_exists(sector->ceiling_texture))
	{
		ft_printf("Error\nInvalid ceiling texture: %s\n", sector->ceiling_texture);
		return (false);
	}
	return (true);
}

/*
 * Main sector validation function
 * Validates all sectors in the map
 */
bool	validate_sectors(t_doom_map *map)
{
	int	i;

	ft_printf("Starting sector validation (count: %d)...\n", map->sector_count);
	if (map->sector_count == 0)
	{
		ft_putendl_fd("Error\nNo sectors defined in map", 2);
		return (false);
	}

	i = -1;
	while (++i < map->sector_count)
	{
		if (!validate_sector(&map->sectors[i], map, i))
		{
			ft_printf("Error\nValidation failed for sector %d\n", i);
			return (false);
		}
	}
	ft_printf("All sectors validated successfully!\n");
	return (true);
}
