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

unsigned int get_portal_color(t_portal_wall *portal, double x, double y, t_game *game)
{
    t_texture *texture;
    int tex_x, tex_y;
    
    if (portal->type == PORTAL_BLUE)
        texture = game->portal_system->gun.blue_texture;
    else
        texture = game->portal_system->gun.orange_texture;
    
    tex_x = (int)(x * texture->width);
    tex_y = (int)(y * texture->height);
    
    unsigned int color = get_texture_pixel(texture, tex_x, tex_y);
    
    // Se a cor for a cor de transparência (rosa), retornar a cor especial
    if (color == 0xFFC0CB)
        return 0xFFC0CB;
    
    return color;
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

bool check_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal)
{
    static int debug_counter = 0;
    (void)game;
    if (debug_counter++ % 600 == 0)
    {
        printf("DEBUG: check_portal_hit - portal pos: (%d, %d), active: %d, state: %d\n", 
            portal->position.x, portal->position.y, portal->active, portal->state);
        printf("DEBUG: check_portal_hit - ray pos: (%d, %d), side: %d\n", 
            ray->map_x, ray->map_y, ray->side);
    }
    if (!is_within_map_bounds(game, ray->map_x, ray->map_y))
        return false;
    if (!portal->active || portal->state != PORTAL_OPEN)
    {
        // printf("DEBUG: Portal not active or not open (active=%d, state=%d)\n", 
        //        portal->active, portal->state);
        return (false);
    }
    
    if (ray->map_x == portal->position.x && ray->map_y == portal->position.y)
    {
        if ((ray->side == 0 && portal->normal.x != 0) ||
            (ray->side == 1 && portal->normal.y != 0))
        {
            printf("DEBUG: Portal hit found at position (%d, %d)\n", 
                   ray->map_x, ray->map_y);
            return (true);
        }
    }
    return (false);
}