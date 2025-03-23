/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/05 15:31:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void link_portals(t_game *game)
{
    t_portal_system *ps;
    
    ps = game->portal_system;
    
    ps->portal_active = ps->blue_portal.active || ps->orange_portal.active;
    
    if (ps->blue_portal.active && ps->orange_portal.active)
    {
        ps->blue_portal.linked_portal = &ps->orange_portal;
        ps->orange_portal.linked_portal = &ps->blue_portal;
    }
    else
    {
        if (ps->blue_portal.active)
            ps->blue_portal.linked_portal = NULL;
            
        if (ps->orange_portal.active)
            ps->orange_portal.linked_portal = NULL;
    }
}

static t_cardinal get_cardinal_from_normal(t_vector normal)
{
    if (normal.x > 0.5)
        return (EAST);
    if (normal.x < -0.5)
        return (WEST);
    if (normal.y > 0.5)
        return (NORTH);
    return (SOUTH);
}

static void	set_portal_properties(t_portal_wall *portal, t_portal_hit hit, 
									t_portal_type type)
{
	portal->position = hit.wall_pos;
	portal->normal = hit.normal;
	portal->offset = hit.offset;
	portal->type = type;
	portal->state = PORTAL_OPENING;
	portal->animation = 0.0;
	portal->active = true;
	portal->timer = 0.0;
    portal->card = get_cardinal_from_normal(hit.normal);
}

void create_portal(t_game *game, t_portal_hit hit, t_portal_type type)
{
    t_portal_system *ps;
    
    if (!is_valid_portal_surface(game, hit))
    {
        return;
    }
    ps = game->portal_system;
    if (type == PORTAL_BLUE)
    {
        set_portal_properties(&ps->blue_portal, hit, PORTAL_BLUE);
    }
    else if (type == PORTAL_ORANGE)
    {
        set_portal_properties(&ps->orange_portal, hit, PORTAL_ORANGE);
    }
    link_portals(game);
}

void    switch_weapon(t_game *game)
{
    if (game->active_weapon == 0)
    {
        game->active_weapon = 1;
    }
    else
    {
        game->active_weapon = 0;
    }
    game->p1.is_firing = 0;
    game->p1.current_frame = 0;
    if (game->portal_system)
    {
        game->portal_system->gun.is_firing = 0;
        game->portal_system->gun.current_frame = 0;
    }
}


void    update_portal_gun_animation(t_game *game)
{
    double      current_time;
    t_portal_gun *gun;
    static int  frame_counter = 0;
    const int   frames_per_animation = 3;

    gun = &game->portal_system->gun;
    current_time = get_time_ms();
    if (gun->is_firing)
    {
        if (current_time - gun->last_fire_time <= 60)
            return ;
        frame_counter++;
        if (frame_counter >= frames_per_animation)
        {
            gun->current_frame = (gun->current_frame + 1) % 4;
            frame_counter = 0;
            if (gun->current_frame == 0)
                gun->is_firing = 0;
        }
        gun->last_fire_time = current_time;
    }
    else
    {
        gun->current_frame = 0;
        frame_counter = 0;
    }
}

void	fire_portal_gun(t_game *game)
{
	t_portal_hit	hit;
	t_portal_type	portal_type;
	t_portal_wall	*other_portal;
	double			current_time;
	t_cardinal		hit_cardinal;

	if (game->active_weapon != 1)
		return ;
	current_time = get_time_ms();
	if (current_time - game->portal_system->gun.last_fire_time <
		game->portal_system->gun.cooldown)
		return ;
	game->portal_system->gun.is_firing = 1;
	game->portal_system->gun.current_frame = 1;
	portal_type = game->portal_system->gun.active_portal;
	hit = detect_wall_for_portal(game);
	if (!hit.found)
		return ;
	if (!is_valid_portal_surface(game, hit))
		return ;
	hit_cardinal = get_cardinal_from_normal(hit.normal);
	if (portal_type == PORTAL_BLUE)
		other_portal = &game->portal_system->orange_portal;
	else
		other_portal = &game->portal_system->blue_portal;
	if (other_portal->active &&
		other_portal->position.x == hit.wall_pos.x &&
		other_portal->position.y == hit.wall_pos.y &&
		(hit_cardinal == other_portal->card ||
		hit_cardinal == get_opposite_cardinal(other_portal->card)))
		return ;
	create_portal(game, hit, portal_type);
	game->portal_system->gun.last_fire_time = current_time;
}
