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

#include "map.h"

/*
 * Initializes an empty map structure
 * Sets all counters to 0 and pointers to NULL
 */
void	init_map(t_doom_map *map)
{
	ft_bzero(map, sizeof(t_doom_map));
}

void	free_split(char **split)
{
	int	i;

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

	i = 0;
	while (i < map->sector_count)
	{
		if (map->sectors[i].linedefs)
		{
			free(map->sectors[i].linedefs);
			map->sectors[i].linedefs = NULL;
		}
		i++;
	}
}
