/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movement.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:30:00 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/23 19:40:00 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	c_enemy_mx(t_game *game, t_vector n_pos, t_vector dir, double padd)
{
	int	map_x;
	int	map_y;

	map_x = (int)(n_pos.x + dir.x * padd);
	map_y = (int)(n_pos.y);
	if (map_x >= 0 && map_x < game->map.width
		&& map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}

bool	c_enemy_my(t_game *game, t_vector n_pos, t_vector dir, double padd)
{
	int	map_x;
	int	map_y;

	map_x = (int)(n_pos.x);
	map_y = (int)(n_pos.y + dir.y * padd);
	if (map_x >= 0 && map_x < game->map.width
		&& map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}
