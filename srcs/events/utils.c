/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 20:00:22 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/09 20:11:22 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	*keys_else(t_game *game, double *dir_x, double *dir_y)
{
	if (game->p1.keys.d)
	{
		*dir_x += game->p1.plane.x;
		*dir_y += game->p1.plane.y;
	}
	if (game->p1.keys.a)
	{
		*dir_x -= game->p1.plane.x;
		*dir_y -= game->p1.plane.y;
	}
	return (NULL);
}
