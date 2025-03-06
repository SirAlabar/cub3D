/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:50:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/05 15:51:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	update_portal_state(t_portal_wall *portal, double delta_time)
{
	const double	animation_speed = 2.0;
	
	if (!portal->active)
		return ;
		
	if (portal->state == PORTAL_OPENING)
	{
		portal->animation += animation_speed * delta_time;
		if (portal->animation >= 1.0)
		{
			portal->animation = 1.0;
			portal->state = PORTAL_OPEN;
		}
	}
	else if (portal->state == PORTAL_CLOSING)
	{
		portal->animation -= animation_speed * delta_time;
		if (portal->animation <= 0.0)
		{
			portal->animation = 0.0;
			portal->state = PORTAL_CLOSED;
			portal->active = false;
		}
	}
}

// void	update_portals(t_game *game)
// {
// 	t_portal_system	*ps;
// 	double			delta_time;
	
// 	ps = game->portal_system;
// 	delta_time = 1.0 / 60.0;
	
// 	if (!ps)
// 		return ;
		
// 	update_portal_state(&ps->blue_portal, delta_time);
// 	update_portal_state(&ps->orange_portal, delta_time);
	
// 	check_portal_teleport(game);
// }

void update_portals(t_game *game)
{
    t_portal_system *ps;
    double delta_time;
    static int debug_counter = 0;
    

    ps = game->portal_system;
    delta_time = 1.0 / 60.0;
    if (!ps)
    return;
    if (debug_counter++ % 100 == 0)
    {
        printf("DEBUG: Updating portals - Blue: %s (%s), Orange: %s (%s)\n", 
               ps->blue_portal.active ? "active" : "inactive",
               ps->blue_portal.state == PORTAL_OPEN ? "open" : 
               (ps->blue_portal.state == PORTAL_OPENING ? "opening" : 
               (ps->blue_portal.state == PORTAL_CLOSING ? "closing" : "closed")),
               ps->orange_portal.active ? "active" : "inactive",
               ps->orange_portal.state == PORTAL_OPEN ? "open" : 
               (ps->orange_portal.state == PORTAL_OPENING ? "opening" : 
               (ps->orange_portal.state == PORTAL_CLOSING ? "closing" : "closed")));
    }
    

    
    update_portal_state(&ps->blue_portal, delta_time);
    update_portal_state(&ps->orange_portal, delta_time);
    
    check_portal_teleport(game);
}

void handle_portal_gun_input(t_game *game, int keycode)
{
    if (!game || !game->portal_system)
        return;
        
    if (keycode == MOUSE_LEFT)
    {
        game->portal_system->gun.active_portal = PORTAL_BLUE;
        fire_portal_gun(game);
    }
    else if (keycode == MOUSE_RIGHT)
    {
        game->portal_system->gun.active_portal = PORTAL_ORANGE;
        fire_portal_gun(game);
    }
    else if (keycode == KEY_Q)
    {
        toggle_portal_gun(game);
    }
}

void	close_portals(t_game *game)
{
	t_portal_system	*ps;
	
	ps = game->portal_system;
	
	if (ps->blue_portal.active && ps->blue_portal.state == PORTAL_OPEN)
	{
		ps->blue_portal.state = PORTAL_CLOSING;
	}
	
	if (ps->orange_portal.active && ps->orange_portal.state == PORTAL_OPEN)
	{
		ps->orange_portal.state = PORTAL_CLOSING;
	}
	
	ps->portal_active = false;
}
