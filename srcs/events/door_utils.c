/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 19:15:30 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_door(char tile)
{
	return (tile == 'D');
}

t_door	*find_door(t_game *game, int x, int y)
{
	int	i;

	if (!game || !game->door_system || !game->door_system->doors)
		return (NULL);
	i = 0;
	while (i < game->door_system->door_count)
	{
		if (game->door_system->doors[i].position.x == x
			&& game->door_system->doors[i].position.y == y)
		{
			return (&game->door_system->doors[i]);
		}
		i++;
	}
	return (NULL);
}
