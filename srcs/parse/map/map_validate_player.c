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
 * Verifies if map has exactly one player thing
 * Player thing is identified by type=1
 * Returns false if no player or multiple players found
 */
bool	validate_player(t_doom_map *map)
{
	int	i;
	int	player_count;

	i = 0;
	player_count = 0;
	while (i < map->thing_count)
	{
		if (map->things[i].type == 1)
			player_count++;
		i++;
	}
	if (player_count != 1)
	{
		ft_putendl_fd("Error\nMap must have exactly one player", 2);
		return (false);
	}
	return (true);
}

/*
 * Validates all required thing constraints
 * Currently checks:
 * - Exactly one player exists
 * Returns false if any validation fails
 */
bool	validate_things(t_doom_map *map)
{
	return (validate_player(map));
}
