/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 15:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 15:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Initializes an empty map structure
 * Sets all counters to 0 and pointers to NULL
 */
void	init_map(t_doom_map *map)
{
	ft_bzero(map, sizeof(t_doom_map));
	map->vertex_count = 0;
	map->sidedef_count = 0;
	map->linedef_count = 0;    
	map->sector_count = 0;
	map->thing_count = 0;
}

/*
 * Free array returned by ft_split
 * Sets pointer to NULL after freeing
 */
void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

/*
 * Frees allocated textures in a sidedef and sets pointers to NULL
 */
static void	cleanup_sidedef_textures(t_sidedef *sidedef)
{
	if (sidedef->upper_texture)
	{
		free(sidedef->upper_texture);
		sidedef->upper_texture = NULL;
	}
	if (sidedef->middle_texture)
	{
		free(sidedef->middle_texture);
		sidedef->middle_texture = NULL;
	}
	if (sidedef->lower_texture)
	{
		free(sidedef->lower_texture);
		sidedef->lower_texture = NULL;
	}
}

/*
 * Frees allocated textures in a sector and sets pointers to NULL
 */
static void	cleanup_sector_textures(t_sector *sector)
{
	if (sector->floor_texture)
	{
		free(sector->floor_texture);
		sector->floor_texture = NULL;
	}
	if (sector->ceiling_texture)
	{
		free(sector->ceiling_texture);
		sector->ceiling_texture = NULL;
	}
	if (sector->linedefs)
	{
		free(sector->linedefs);
		sector->linedefs = NULL;
	}
}

/*
 * Frees all allocated memory in map structure
 * Cleans up sector linedef arrays
 * Sets all pointers to NULL after freeing
 */
void	cleanup_map(t_doom_map *map)
{
	int	i;

	i = -1;
	while (++i < map->sector_count)
		cleanup_sector_textures(&map->sectors[i]);
	i = -1;
	while (++i < map->sidedef_count)
		cleanup_sidedef_textures(&map->sidedefs[i]);
}
