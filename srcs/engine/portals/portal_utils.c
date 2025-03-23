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

unsigned int	get_portal_color(t_portal_wall *portal, double x, double y,
		t_game *game)
{
	t_texture		*texture;
	int				tex_x;
	int				tex_y;
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

bool	check_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal)
{
	t_cardinal	ray_direction;

	(void)game;
	if (!portal->active || (portal->state != PORTAL_OPEN
			&& portal->state != PORTAL_OPENING))
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
	if (ray_direction == portal->card)
		return (true);
	return (false);
}
