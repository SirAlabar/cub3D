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

static double	get_wall_distance(int check_pos, double new_pos, double grid_pos)
{
	if (check_pos > grid_pos)
		return (check_pos - new_pos);
	return (new_pos - grid_pos);
}

static int	get_offset(double dir)
{
	if (dir > 0)
		return (1);
	return (-1);
}

static bool check_wall_collision(t_game *g, int pos1, int pos2)
{
    char tile;
    
    tile = g->map.grid[pos1][pos2];    
    if (tile == '1')
        return (true);
    if (tile == 'D')
    {
        // Usa as mesmas coordenadas que encontrou o 'D'
        t_door *door = find_door(g, pos1, pos2);
        if (door && door->state == DOOR_OPEN && door->animation >= 0.99)
            return (false);
        return (true);
    }
    return (false);
}

bool	can_move_x(t_game *g, t_vector new, t_vector dir, double buff)
{
	int		new_x_pos;
	int		y_pos;
	int		offset;
	int		check_x;

	new_x_pos = (int)new.x;
	y_pos = (int)g->p1.pos.y;
	offset = get_offset(dir.x);
	if (check_wall_collision(g, new_x_pos + offset * (int)0.2, y_pos))
		return (false);
	check_x = new_x_pos + offset;
	if (check_wall_collision(g, check_x, y_pos))
		if (get_wall_distance(check_x, new.x, new_x_pos) < buff)
			return (false);
	return (true);
}

bool	can_move_y(t_game *g, t_vector new, t_vector dir, double buff)
{
	int		new_y_pos;
	int		x_pos;
	int		offset;
	int		check_y;
    

	new_y_pos = (int)new.y;
	x_pos = (int)g->p1.pos.x;
	offset = get_offset(dir.y);
	if (check_wall_collision(g, x_pos, new_y_pos + offset * (int)0.2))
		return (false);
	check_y = new_y_pos + offset;
	if (check_wall_collision(g, x_pos, check_y))
		if (get_wall_distance(check_y, new.y, new_y_pos) < buff)
			return (false);
	return (true);
}