/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:48:09 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_cardinal	get_opposite_cardinal(t_cardinal card)
{
	if (card == NORTH)
		return (SOUTH);
	if (card == SOUTH)
		return (NORTH);
	if (card == EAST)
		return (WEST);
	return (EAST);
}

double	get_angle_between_normals(t_vector n1, t_vector n2)
{
	double	dot;
	double	angle;

	dot = vector_dot(n1, n2);
	if (dot > 1.0)
		dot = 1.0;
	if (dot < -1.0)
		dot = -1.0;
	angle = acos(dot);
	if (n1.x * n2.y - n1.y * n2.x < 0)
		angle = -angle;
	return (angle);
}

t_vector	get_teleport_position(t_portal_wall *exit_portal)
{
	t_vector	pos;

	pos.x = exit_portal->position.x + exit_portal->normal.x * 0.8 + 0.5;
	pos.y = exit_portal->position.y + exit_portal->normal.y * 0.8 + 0.5;
	return (pos);
}

bool	is_within_map_bounds(t_game *game, int x, int y)
{
	return (x >= 0 && y >= 0 && x < game->map.height && y < game->map.width);
}
