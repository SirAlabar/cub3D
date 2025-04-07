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

bool	can_move_x(t_game *g, t_vector new, t_vector dir, double buff)
{
	int		new_grid_x_pos;
	int		grid_y_pos;
	int		offset;
	int		check_x;
	double	dist_to_wall;

	new_grid_x_pos = (int)new.x;
	grid_y_pos = (int)g->p1.pos.y;
	if (dir.x > 0)
		offset = 1;
	else
		offset = -1;
	if (g->map.grid[new_grid_x_pos + offset * (int)0.2][grid_y_pos] == '1')
		return (false);
	check_x = new_grid_x_pos + offset;
	if (g->map.grid[check_x][grid_y_pos] == '1')
	{
		if (dir.x > 0)
			dist_to_wall = check_x - new.x;
		else
			dist_to_wall = new.x - new_grid_x_pos;
		if (dist_to_wall < buff)
			return (false);
	}
	return (true);
}

bool	can_move_y(t_game *g, t_vector new, t_vector dir, double buff)
{
	int		new_grid_y_pos;
	int		grid_x_pos;
	int		offset;
	int		check_y;
	double	dist_to_wall;

	new_grid_y_pos = (int)new.y;
	grid_x_pos = (int)g->p1.pos.x;
	if (dir.y > 0)
		offset = 1;
	else
		offset = -1;
	if (g->map.grid[grid_x_pos][new_grid_y_pos + offset * (int)0.2] == '1')
		return (false);
	check_y = new_grid_y_pos + offset;
	if (g->map.grid[grid_x_pos][check_y] == '1')
	{
		if (dir.y > 0)
			dist_to_wall = check_y - new.y;
		else
			dist_to_wall = new.y - new_grid_y_pos;
		if (dist_to_wall < buff)
			return (false);
	}
	return (true);
}

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
