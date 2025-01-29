/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:44:29 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/29 17:45:08 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_in_bounds(t_game *game, int x, int y)
{
	return (x >= 0 && x < game->map.width && y >= 0 && y < game->map.height);
}

bool	has_hit_wall(t_game *game, t_ray_data *ray) {

	return (is_in_bounds(game, ray->map_x, ray->map_y)
		&& game->map.grid[ray->map_y][ray->map_x] == '1');
}
