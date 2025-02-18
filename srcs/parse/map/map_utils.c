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
	map->skybox_path = NULL;
}

int    world_to_map(int x)
{
    return (x / TILE_SIZE);
}

int    map_to_world(int x)
{
    return (x * TILE_SIZE);
}

t_fixed_vec32    get_map_center(t_doom_map *map)
{
    t_fixed_vec32 center;
    int i;
    int min_x, max_x;
    int min_y, max_y;

    min_x = fixed32_to_int(map->vertices[0].pos.x);
    max_x = min_x;
    min_y = fixed32_to_int(map->vertices[0].pos.y);
    max_y = min_y;

    i = 1;
    while (i < map->vertex_count)
    {
        int x = fixed32_to_int(map->vertices[i].pos.x);
        int y = fixed32_to_int(map->vertices[i].pos.y);

        if (x < min_x)
            min_x = x;
        if (x > max_x)
            max_x = x;
        if (y < min_y)
            min_y = y;
        if (y > max_y)
            max_y = y;
        i++;
    }

    center.x = int_to_fixed32((max_x + min_x) / 2);
    center.y = int_to_fixed32((max_y + min_y) / 2);
    
    return (center);
}

t_fixed_vec32    center_coords(t_fixed_vec32 pos, t_doom_map *map)
{
    t_fixed_vec32 center;
    t_fixed_vec32 centered_pos;

    center = get_map_center(map);
    centered_pos.x = fixed32_sub(pos.x, center.x);
    centered_pos.y = fixed32_sub(pos.y, center.y);

    centered_pos.x = fixed32_mul(centered_pos.x, int_to_fixed32(TILE_SIZE));
    centered_pos.y = fixed32_mul(centered_pos.y, int_to_fixed32(TILE_SIZE));

    return (centered_pos);
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
