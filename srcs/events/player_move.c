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
	double		speed;

	if (g->p1.keys.shift)
		speed = g->p1.move_speed * 2.0;
	else
		speed = g->p1.move_speed;
	visual_buff = 0.5;
	dir.x = dir_x;
	dir.y = dir_y;
	new_pos.x = g->p1.pos.x + dir.x * speed;
	new_pos.y = g->p1.pos.y + dir.y * speed;
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

static void	handle_player_movement(t_game *game)
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
}

void	handle_movement(t_game *game)
{
	static double	last_footstep_time = 0;
	double			current_time;
	int				is_moving;
	double			footstep_interval;

	handle_player_movement(game);
	is_moving = (game->p1.keys.w || game->p1.keys.s || game->p1.keys.a
			|| game->p1.keys.d);
	if (is_moving && game->sounds && game->sounds->footstep)
	{
		current_time = get_time_ms();
		if (game->p1.keys.shift)
			footstep_interval = 175;
		else
			footstep_interval = 350;
		if (current_time - last_footstep_time > footstep_interval)
		{
			play_sound(game->sounds->footstep);
			last_footstep_time = current_time;
		}
	}
	if (game->p1.keys.left)
		rotate_player(game, -game->p1.rot_speed);
	if (game->p1.keys.right)
		rotate_player(game, game->p1.rot_speed);
}
