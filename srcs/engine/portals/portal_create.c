/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:47:29 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:47:32 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	link_portals(t_game *game)
{
	t_portal_system	*ps;

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

t_cardinal	get_cardinal_from_normal(t_vector normal)
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

void	create_portal(t_game *game, t_portal_hit hit, t_portal_type type)
{
	t_portal_system	*ps;

	if (!is_valid_portal_surface(game, hit))
	{
		return ;
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

void	init_hit(t_portal_hit *hit)
{
	hit->found = false;
	hit->wall_pos = vector_i_create(0, 0);
	hit->hit_pos = vector_create(0, 0);
	hit->normal = vector_create(0, 0);
	hit->offset = 0.0;
	hit->distance = 0.0;
}
