/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/07 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_cardinal	get_opposite_cardinal(t_cardinal card)
{
	if (card == NORTH)
		return (SOUTH);
	if (card == SOUTH)
		return (NORTH);
	if (card == EAST)
		return (WEST);
	return (EAST);
}

t_vector	get_direction_vector(t_cardinal card)
{
	t_vector	dir;

	dir.x = 0;
	dir.y = 0;
	if (card == NORTH)
		dir.x = 1;
	else if (card == SOUTH)
		dir.x = -1;
	else if (card == EAST)
		dir.y = 1;
	else
		dir.y = -1;
	return (dir);
}

double	get_angle_between_normals(t_vector n1, t_vector n2)
{
	double	dot;
	double	angle;
	
	dot = vector_dot(n1, n2);
	if (dot > 1.0)
		dot = 1.0;
	if (dot < -1.0)
		dot = -1.0;
	angle = acos(dot);
	if (n1.x * n2.y - n1.y * n2.x < 0)
		angle = -angle;
	return (angle);
}

t_vector	get_teleport_position(t_portal_wall *exit_portal)
{
	t_vector	pos;
	
	pos.x = exit_portal->position.x + exit_portal->normal.x * 0.8 + 0.5;
	pos.y = exit_portal->position.y + exit_portal->normal.y * 0.8 + 0.5;
	return (pos);
}

void	adjust_player_orientation(t_game *game, t_portal_wall *entry, 
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

bool	is_near_portal(t_game *game, t_portal_wall *portal)
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
	if (distance > PORTAL_DIST_THRESHOLD)
		return (false);
	to_portal = vector_normalize(to_portal);
	dot = vector_dot(game->p1.dir, to_portal);
	return (dot > PORTAL_DOT_THRESHOLD);
}

unsigned int	get_portal_color(t_portal_wall *portal, double x, double y,
	t_game *game)
{
	t_texture	*texture;
	int			tex_x;
	int			tex_y;
	unsigned int	color;

	if (portal->type == PORTAL_BLUE)
		texture = game->portal_system->gun.blue_texture;
	else
		texture = game->portal_system->gun.orange_texture;
	tex_x = (int)(x * texture->width);
	tex_y = (int)(y * texture->height);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	color = get_texture_pixel(texture, tex_x, tex_y);
	return (color);
}

// bool	check_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal)
// {
//     (void)game;
// 	if (!portal->active || portal->state != PORTAL_OPEN)
// 		return (false);
// 	if (ray->map_x == portal->position.x && ray->map_y == portal->position.y)
// 	{
// 		if (ray->side == 0 && portal->normal.x != 0)
// 			return (true);
// 		else if (ray->side == 1 && portal->normal.y != 0)
// 			return (true);
// 	}
// 	return (false);
// }

bool	check_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal)
{
	t_cardinal	ray_direction;
	static int debug_counter2 = 0;

	(void)game;
	if (debug_counter2++ % 90000 == 0){
	printf("DEBUG: Checking portal hit at pos (%d,%d) - Portal pos (%d,%d)\n", 
		ray->map_x, ray->map_y, portal->position.x, portal->position.y);
 	printf("DEBUG: Portal active: %d, state: %d, card: %d\n", 
		portal->active, portal->state, portal->card);}	
	if (!portal->active || (portal->state != PORTAL_OPEN && portal->state != PORTAL_OPENING))
		return (false);
	if (ray->map_x != portal->position.x || ray->map_y != portal->position.y)
		return (false);
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			ray_direction = WEST;
		else
			ray_direction = EAST;
	}
	else
	{
		if (ray->dir.y > 0)
			ray_direction = SOUTH;
		else
			ray_direction = NORTH;
	}
	if (debug_counter2++ % 90000 == 0){
	printf("DEBUG: Ray direction = %d, Portal card = %d, Match = %d\n", 
		ray_direction, portal->card, (ray_direction == portal->card));}
	if (ray_direction == portal->card)
		return (true);
	return (false);
}
