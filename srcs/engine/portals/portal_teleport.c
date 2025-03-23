/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_teleport.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:40:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/05 15:41:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>


static t_vector	get_teleport_position(t_portal_wall *exit_portal)
{
	t_vector	pos;
	double		offset;
	
	offset = 1.2;
	pos.x = exit_portal->position.x + 0.5 + exit_portal->normal.x * offset;
	pos.y = exit_portal->position.y + 0.5 + exit_portal->normal.y * offset;
	return (pos);
}

static void	adjust_player_orientation(t_game *game, t_portal_wall *entry, 
	t_portal_wall *exit)
{
	double		angle;
	t_vector	old_dir;
	t_vector	old_plane;
	t_vector	exit_normal;
	double		dot;

	angle = get_angle_between_normals(entry->normal, exit->normal);
	old_dir = game->p1.dir;
	old_plane = game->p1.plane;
	game->p1.dir = vector_rotate(old_dir, angle);
	game->p1.plane = vector_rotate(old_plane, angle);
	exit_normal = exit->normal;
	dot = vector_dot(game->p1.dir, exit_normal);
	if (dot < 0)
	{
		game->p1.dir = vector_mult(exit_normal, 1.0);
		game->p1.plane = vector_rotate(game->p1.dir, M_PI/2);
		game->p1.plane = vector_mult(game->p1.plane, 0.66);
	}
}

void check_portal_teleport(t_game *game)
{
    t_portal_system *ps;
    t_vector next_pos;
    t_vector dir_normalized;
    double current_time;
    int map_x, map_y;
    
    ps = game->portal_system;
    current_time = get_time_ms();
    
    if (!ps->portal_active || current_time - ps->last_teleport_time < 500.0)
        return;
    dir_normalized = vector_normalize(game->p1.dir);
    next_pos.x = game->p1.pos.x + dir_normalized.x * 0.1;
    next_pos.y = game->p1.pos.y + dir_normalized.y * 0.1;
    map_x = (int)next_pos.x;
    map_y = (int)next_pos.y;
    if (is_within_map_bounds(game, map_x, map_y) && game->map.grid[map_x][map_y] == '1')
    {
        if (ps->blue_portal.active && ps->blue_portal.linked_portal && 
            ps->blue_portal.state == PORTAL_OPEN &&
            ps->blue_portal.position.x == map_x && 
            ps->blue_portal.position.y == map_y)
        {
            game->p1.pos = get_teleport_position(ps->blue_portal.linked_portal);
            adjust_player_orientation(game, &ps->blue_portal, ps->blue_portal.linked_portal);
            ps->last_teleport_time = current_time;
            printf("DEBUG: Teleported through blue portal\n");
            return;
        }
        if (ps->orange_portal.active && ps->orange_portal.linked_portal && 
            ps->orange_portal.state == PORTAL_OPEN &&
            ps->orange_portal.position.x == map_x && 
            ps->orange_portal.position.y == map_y)
        {
            game->p1.pos = get_teleport_position(ps->orange_portal.linked_portal);
            adjust_player_orientation(game, &ps->orange_portal, ps->orange_portal.linked_portal);
            ps->last_teleport_time = current_time;
            printf("DEBUG: Teleported through orange portal\n");
            return;
        }
    }
}