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
}

// void	create_portal(t_game *game, t_portal_hit hit, t_portal_type type)
// {
// 	t_portal_system	*ps;
	
// 	if (!is_valid_portal_surface(game, hit))
// 		return ;
		
// 	ps = game->portal_system;
	
// 	if (type == PORTAL_BLUE)
// 	{
// 		set_portal_properties(&ps->blue_portal, hit, PORTAL_BLUE);
// 	}
// 	else if (type == PORTAL_ORANGE)
// 	{
// 		set_portal_properties(&ps->orange_portal, hit, PORTAL_ORANGE);
// 	}
	
// 	link_portals(game);
// }

void create_portal(t_game *game, t_portal_hit hit, t_portal_type type)
{
    t_portal_system *ps;
    
    printf("DEBUG: Creating portal\n");
    
    if (!is_valid_portal_surface(game, hit))
    {
        printf("DEBUG: Invalid portal surface\n");
        return;
    }
        
    ps = game->portal_system;
    
    if (type == PORTAL_BLUE)
    {
        printf("DEBUG: Setting blue portal properties\n");
        set_portal_properties(&ps->blue_portal, hit, PORTAL_BLUE);
    }
    else if (type == PORTAL_ORANGE)
    {
        printf("DEBUG: Setting orange portal properties\n");
        set_portal_properties(&ps->orange_portal, hit, PORTAL_ORANGE);
    }
    
    link_portals(game);
    printf("DEBUG: Portals linked: %s\n", ps->portal_active ? "YES" : "NO");
}

void	toggle_portal_gun(t_game *game)
{
	t_portal_system	*ps;
	
	ps = game->portal_system;
	
	if (ps->gun.active_portal == PORTAL_BLUE)
		ps->gun.active_portal = PORTAL_ORANGE;
	else
		ps->gun.active_portal = PORTAL_BLUE;
}

// void	fire_portal_gun(t_game *game)
// {
// 	t_portal_hit	hit;
// 	t_portal_type	portal_type;
// 	double			current_time;
	
// 	current_time = get_time_ms();
	
// 	if (current_time - game->portal_system->gun.last_fire_time < 
// 		game->portal_system->gun.cooldown)
// 		return ;
		
// 	hit = detect_wall_for_portal(game);
	
// 	if (!hit.found)
// 		return ;
		
// 	portal_type = game->portal_system->gun.active_portal;
// 	create_portal(game, hit, portal_type);
	
// 	game->portal_system->gun.last_fire_time = current_time;
// }

void fire_portal_gun(t_game *game)
{
    t_portal_hit    hit;
    t_portal_type   portal_type;
    double          current_time;
    
    printf("DEBUG: Firing portal gun\n");
    
    current_time = get_time_ms();
    
    if (current_time - game->portal_system->gun.last_fire_time < 
        game->portal_system->gun.cooldown)
    {
        printf("DEBUG: Portal gun on cooldown\n");
        return;
    }
        
    hit = detect_wall_for_portal(game);
    
    printf("DEBUG: Portal hit found: %s\n", hit.found ? "YES" : "NO");
    if (hit.found)
    {
        printf("DEBUG: Wall position: (%d, %d)\n", hit.wall_pos.x, hit.wall_pos.y);
        printf("DEBUG: Hit position: (%.2f, %.2f)\n", hit.hit_pos.x, hit.hit_pos.y);
        printf("DEBUG: Normal: (%.2f, %.2f)\n", hit.normal.x, hit.normal.y);
        printf("DEBUG: Distance: %.2f\n", hit.distance);
    }
    
    if (!hit.found)
        return;
        
    printf("DEBUG: Checking if surface is valid\n");
    if (!is_valid_portal_surface(game, hit))
    {
        printf("DEBUG: Invalid portal surface\n");
        return;
    }
    
    portal_type = game->portal_system->gun.active_portal;
    printf("DEBUG: Creating portal of type: %s\n", 
           portal_type == PORTAL_BLUE ? "BLUE" : "ORANGE");
    
    create_portal(game, hit, portal_type);
    
    game->portal_system->gun.last_fire_time = current_time;
    printf("DEBUG: Portal gun fired successfully\n");
}