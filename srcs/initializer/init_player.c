/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:55:26 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/06 19:29:47 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_player(t_game *game)
{
	game->p1.pos = vector_create(game->p1.pos.x, game->p1.pos.y);
	game->p1.dir = vector_create(0, 1);
	game->p1.plane = vector_create(0.66, 0);
	game->p1.move_speed = MOVE_SPEED;
	game->p1.rot_speed = ROTATION_SPEED;
	game->p1.keys = (t_keys){0, 0, 0, 0, 0, 0};
}
