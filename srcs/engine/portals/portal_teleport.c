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
	
	offset = 0.8;
	
	pos.x = exit_portal->position.x + exit_portal->normal.x * offset;
	pos.y = exit_portal->position.y + exit_portal->normal.y * offset;
	
	return (pos);
}

static void	adjust_player_orientation(t_game *game, t_portal_wall *entry, 
									t_portal_wall *exit)
{
	double		angle;
	t_vector	old_dir;
	t_vector	old_plane;
	
	angle = get_angle_between_normals(entry->normal, exit->normal);
	
	if (angle != 0.0)
	{
		old_dir = game->p1.dir;
		old_plane = game->p1.plane;
		
		game->p1.dir = vector_rotate(old_dir, angle);
		game->p1.plane = vector_rotate(old_plane, angle);
	}
}

static bool	is_near_portal(t_game *game, t_portal_wall *portal)
{
	t_vector	player_pos;
	t_vector	portal_pos;
	t_vector	to_portal;
	double		distance;
	double		dot;
	
	if (!portal->active || !portal->linked_portal || 
		portal->state != PORTAL_OPEN)
		return (false);
	
	player_pos = game->p1.pos;
	portal_pos = vector_create(portal->position.x + 0.5, 
							 portal->position.y + 0.5);
	
	to_portal = vector_sub(portal_pos, player_pos);
	distance = vector_mag(to_portal);
	
	if (distance > 1.0)
		return (false);
		
	to_portal = vector_normalize(to_portal);
	dot = vector_dot(game->p1.dir, to_portal);
	
	return (dot > 0.5);
}

void	check_portal_teleport(t_game *game)
{
	t_portal_system	*ps;
	double			current_time;
	
	ps = game->portal_system;
	current_time = get_time_ms();
	
	if (!ps->portal_active || 
		current_time - ps->last_teleport_time < 1000.0)
		return ;
		
	if (is_near_portal(game, &ps->blue_portal))
	{
		game->p1.pos = get_teleport_position(ps->blue_portal.linked_portal);
		adjust_player_orientation(game, &ps->blue_portal, 
								ps->blue_portal.linked_portal);
		ps->last_teleport_time = current_time;
	}
	else if (is_near_portal(game, &ps->orange_portal))
	{
		game->p1.pos = get_teleport_position(ps->orange_portal.linked_portal);
		adjust_player_orientation(game, &ps->orange_portal, 
								ps->orange_portal.linked_portal);
		ps->last_teleport_time = current_time;
	}
}
