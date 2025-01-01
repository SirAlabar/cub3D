/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:55:26 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/01 18:20:33 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_player(t_game *game)
{
	game->p1.pos = (t_vector){0, 0};
	game->p1.dir = (t_vector){0, 0};
	game->p1.plane = (t_vector){0, 0};
	game->p1.move_speed = MOVE_SPEED;
	game->p1.rot_speed = ROTATION_SPEED;
}
