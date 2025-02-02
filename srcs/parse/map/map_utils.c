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
 * Frees all allocated memory in map structure
 * Cleans up sector linedef arrays
 * Sets all pointers to NULL after freeing
 */
void	cleanup_map(t_doom_map *map)
{
	int	i;

    i = -1;
    while (++i < map->sector_count)
    {
        if (map->sectors[i].floor_texture)
        {
            free(map->sectors[i].floor_texture);
            map->sectors[i].floor_texture = NULL;
        }
        if (map->sectors[i].ceiling_texture)
        {
            free(map->sectors[i].ceiling_texture);
            map->sectors[i].ceiling_texture = NULL;
        }
        if (map->sectors[i].linedefs)
        {
            free(map->sectors[i].linedefs);
            map->sectors[i].linedefs = NULL;
        }
    }
}
