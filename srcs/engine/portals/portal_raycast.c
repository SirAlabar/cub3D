/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_raycast.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:10:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/07 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_portal_wall	*find_portal_at_position(t_game *game, int x, int y,
		t_cardinal card)
{
	if (game->portal_system->blue_portal.active
		&& game->portal_system->blue_portal.position.x == x
		&& game->portal_system->blue_portal.position.y == y
		&& game->portal_system->blue_portal.card == card)
		return (&game->portal_system->blue_portal);
	if (game->portal_system->orange_portal.active
		&& game->portal_system->orange_portal.position.x == x
		&& game->portal_system->orange_portal.position.y == y
		&& game->portal_system->orange_portal.card == card)
		return (&game->portal_system->orange_portal);
	return (NULL);
}

static void	save_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal)
{
	ray->portal.map_x = ray->map_x;
	ray->portal.map_y = ray->map_y;
	ray->portal.perp_wall_dist = ray->perp_wall_dist;
	ray->portal.side = ray->side;
	ray->hit_portal = portal;
	ray->portal.tex_x = ray->tex_x;
	ray->portal.wall_x = get_wall_x(ray, game, ray->perp_wall_dist);
	ray->portal.dir = ray->dir;
}

static t_cardinal	get_ray_direction(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			return (WEST);
		return (EAST);
	}
	else
	{
		if (ray->dir.y > 0)
			return (SOUTH);
		return (NORTH);
	}
}

bool	process_portal_hit(t_ray *ray, t_game *game)
{
	t_portal_wall	*portal;
	t_cardinal		card;

	card = get_ray_direction(ray);
	portal = find_portal_at_position(game, ray->map_x, ray->map_y, card);
	if (!portal || !portal->linked_portal || !portal->linked_portal->active)
		return (false);
	if (ray->portal_depth == 0)
	{
		save_portal_hit(ray, game, portal);
		return (false);
	}
	if (ray->portal_depth >= MAX_PORTAL_DEPTH)
		return (false);
	ray->portal_depth++;
	return (true);
}
