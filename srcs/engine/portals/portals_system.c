/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portals_system.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:56:04 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/12 18:49:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void init_portal_system(t_game *game)
{
    game->map.portals = ft_calloc(MAX_PORTALS, sizeof(t_portal));
    if (!game->map.portals)
        return ;
    game->map.portal_count = 0;
}

void init_portal(t_game *game, t_vector_i pos, t_vector_i dest, float rot)
{
    t_portal *portal;
        
    if (game->map.portal_count >= MAX_PORTALS)
        return;
    portal = &game->map.portals[game->map.portal_count];
    
    // Portal de ida
    portal->pos = pos;
    portal->dest = dest;
    
    // Portal reverso
    portal->rev_pos = dest;  // A posição reversa é o destino do portal original
    portal->rev_dest = pos;  // O destino reverso é a posição original
    
    portal->rot_offset = rot;
    
    game->map.portal_count++;
}

t_portal *get_portal_pos(t_game *game, t_vector_i pos)
{
    int i;
    t_portal *p;

    i = -1;
    while (++i < game->map.portal_count)
    {
        p = &game->map.portals[i];
        if ((p->pos.x == pos.x && p->pos.y == pos.y) ||
            (p->rev_pos.x == pos.x && p->rev_pos.y == pos.y))
            return p;
    }
    return NULL;
}
/*
void handler_portal(t_game *game)
{
    t_vector_i curr_pos;
    t_portal *portal;

    curr_pos = vector_i_create((int)game->p1.pos.x, (int)game->p1.pos.y);
    portal = get_portal_pos(game, curr_pos);
    if (portal)
    {
        game->p1.pos.x = portal->dest.x + 0.5f;
        game->p1.pos.y = portal->dest.y + 0.5f;
        if (portal->rot_offset != 0)
        {
            game->p1.dir = vector_rotate(game->p1.dir, portal->rot_offset);
            game->p1.plane = vector_rotate(game->p1.plane, portal->rot_offset);
        }
    }
}*/

void handler_portal(t_game *game)
{
    t_vector_i curr_pos;
    t_portal *portal;
    
    curr_pos = vector_i_create((int)game->p1.pos.x, (int)game->p1.pos.y);
    portal = get_portal_pos(game, curr_pos);
    if (portal)
    {
        printf("Antes do teleporte: pos(%.2f, %.2f)\n", game->p1.pos.x, game->p1.pos.y);      
        if (portal->pos.x == curr_pos.x && portal->pos.y == curr_pos.y)
        {
            game->p1.pos.x = portal->dest.x + 0.5f;
            game->p1.pos.y = portal->dest.y + 0.5f;
            if (portal->rot_offset != 0)
            {
                t_vector old_dir = game->p1.dir;
                t_vector old_plane = game->p1.plane;
                game->p1.dir = vector_rotate(old_dir, portal->rot_offset);
                game->p1.plane = vector_rotate(old_plane, portal->rot_offset);
            }
        }
        else if (portal->rev_pos.x == curr_pos.x && portal->rev_pos.y == curr_pos.y)
        {
            game->p1.pos.x = portal->rev_dest.x + 0.5f;
            game->p1.pos.y = portal->rev_dest.y + 0.5f;
            
            if (portal->rot_offset != 0)
            {
                t_vector old_dir = game->p1.dir;
                t_vector old_plane = game->p1.plane;
                game->p1.dir = vector_rotate(old_dir, -portal->rot_offset);
                game->p1.plane = vector_rotate(old_plane, -portal->rot_offset);
            }
        }
        
        printf("Depois do teleporte: pos(%.2f, %.2f)\n", game->p1.pos.x, game->p1.pos.y);
    }
}


