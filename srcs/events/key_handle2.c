/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:11:37 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:11:54 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	handle_firing(t_game *game)
{
	if (game->p1.is_firing)
		shoot_enemy(game);
}

void	handle_rotation(t_game *game)
{
	if (game->p1.keys.left)
		rotate_player(game, -game->p1.rot_speed);
	if (game->p1.keys.right)
		rotate_player(game, game->p1.rot_speed);
}

void	handle_movement(t_game *game)
{
	double	dir_x;
	double	dir_y;

	dir_x = 0;
	dir_y = 0;
	if (game->p1.keys.w)
	{
		dir_x += game->p1.dir.x;
		dir_y += game->p1.dir.y;
	}
	if (game->p1.keys.s)
	{
		dir_x -= game->p1.dir.x;
		dir_y -= game->p1.dir.y;
	}
	keys_else(game, &dir_x, &dir_y);
	move_player(game, dir_x, dir_y);
	handle_rotation(game);
	handle_firing(game);
}
