/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:29 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:55:31 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	move_player(t_game *g, double dir_x, double dir_y)
{
	t_vector	new_pos;
	t_vector	dir;
	double		visual_buff;

	visual_buff = 0.5;
	dir.x = dir_x;
	dir.y = dir_y;
	new_pos.x = g->p1.pos.x + dir.x * g->p1.move_speed;
	new_pos.y = g->p1.pos.y + dir.y * g->p1.move_speed;
	if (can_move_x(g, new_pos, dir, visual_buff))
		g->p1.pos.x = new_pos.x;
	if (can_move_y(g, new_pos, dir, visual_buff))
		g->p1.pos.y = new_pos.y;
}

void	rotate_player(t_game *game, double angle)
{
	t_vector	old_dir;
	t_vector	old_plane;

	old_dir = game->p1.dir;
	old_plane = game->p1.plane;
	game->p1.dir = vector_normalize(vector_rotate(old_dir, angle));
	game->p1.plane = vector_rotate(old_plane, angle);
}
