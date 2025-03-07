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

/*
** Verifica se o raio atingiu um portal e trata a teleportação
** Retorna true se o raio foi teleportado
*/
bool	process_portal_hit(t_ray *ray, t_game *game)
{
	t_portal_wall	*entry;
	t_portal_wall	*exit;

	if (check_portal_hit(ray, game, &game->portal_system->blue_portal))
	{
		entry = &game->portal_system->blue_portal;
		if (entry->linked_portal && entry->linked_portal->active)
		{
			exit = entry->linked_portal;
			teleport_ray(ray, game, entry, exit);
			return (true);
		}
	}
	else if (check_portal_hit(ray, game, &game->portal_system->orange_portal))
	{
		entry = &game->portal_system->orange_portal;
		if (entry->linked_portal && entry->linked_portal->active)
		{
			exit = entry->linked_portal;
			teleport_ray(ray, game, entry, exit);
			return (true);
		}
	}
	return (false);
}

static void	calculate_teleport_angle(t_portal_wall *entry, t_portal_wall *exit,
	double *angle)
{
	*angle = 0;
	if (entry->card == get_opposite_cardinal(exit->card))
		*angle = M_PI;
	else if ((entry->card == NORTH && exit->card == EAST)
		|| (entry->card == EAST && exit->card == SOUTH)
		|| (entry->card == SOUTH && exit->card == WEST)
		|| (entry->card == WEST && exit->card == NORTH))
		*angle = M_PI / 2;
	else
		*angle = -M_PI / 2;
}

void	calculate_ray_steps(t_ray *ray, t_vector adj_pos)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (ray->map_x - adj_pos.x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - adj_pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (ray->map_y - adj_pos.y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - adj_pos.y) * ray->delta_dist.y;
	}
}

void	teleport_ray_update_position(t_ray *ray, t_vector adj_pos)
{
	ray->map_x = (int)adj_pos.x;
	ray->map_y = (int)adj_pos.y;
	ray->delta_dist.x = fabs(1 / ray->dir.x);
	ray->delta_dist.y = fabs(1 / ray->dir.y);
	calculate_ray_steps(ray, adj_pos);
	ray->hit = false;
	ray->is_door = false;
}

void	teleport_ray(t_ray *ray, t_game *game, t_portal_wall *entry,
    t_portal_wall *exit)
{
    t_vector	exit_dir;
    t_vector	adj_pos;
    double		angle;
    t_vector	old_dir;

    exit_dir = get_direction_vector(exit->card);
    adj_pos.x = exit->position.x + exit_dir.x * 0.1 + 0.5;
    adj_pos.y = exit->position.y + exit_dir.y * 0.1 + 0.5;
    if (!is_within_map_bounds(game, (int)adj_pos.x, (int)adj_pos.y))
    {
        ray->hit = true;
        return ;
    }
    calculate_teleport_angle(entry, exit, &angle);
    old_dir = ray->dir;
    ray->dir.x = old_dir.x * cos(angle) - old_dir.y * sin(angle);
    ray->dir.y = old_dir.x * sin(angle) + old_dir.y * cos(angle);
    teleport_ray_update_position(ray, adj_pos);
}