/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:33:10 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	update_door_state(t_door *door, double delta_time)
{
	if (door->state == DOOR_OPENING)
	{
		door->animation += DOOR_SPEED * delta_time;
		if (door->animation >= 1.0)
		{
			door->animation = 1.0;
			door->state = DOOR_OPEN;
			door->timer = DOOR_STAY_OPEN_TIME;
		}
	}
	else if (door->state == DOOR_OPEN)
	{
		door->timer -= delta_time;
		if (door->timer <= 0)
			door->state = DOOR_CLOSING;
	}
	else if (door->state == DOOR_CLOSING)
	{
		door->animation -= DOOR_SPEED * delta_time;
		if (door->animation <= 0.0)
		{
			door->animation = 0.0;
			door->state = DOOR_CLOSED;
		}
	}
}

void	update_doors(t_game *game)
{
	t_door_system	*ds;
	double			delta_time;
	int				i;

	if (!game)
		return ;
	ds = game->door_system;
	if (!ds)
		return ;
	delta_time = 1.0 / 60.0;
	i = 0;
	while (i < ds->door_count)
	{
		if (ds->doors && i < ds->door_count)
		{
			update_door_state(&ds->doors[i], delta_time);
		}
		i++;
	}
}

static void	process_door_animation(t_ray *ray, double door_hit_pos,
		t_door *door)
{
	double	orig_dist;

	if (ray->side == 0)
		orig_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		orig_dist = ray->side_dist.y - ray->delta_dist.y;
	if (door->animation > 0.9 && door_hit_pos > door->animation)
	{
		update_ray_position(ray);
		return ;
	}
	if (door_hit_pos <= door->animation)
	{
		update_ray_position(ray);
		return ;
	}
	ray->hit = true;
	ray->is_door = true;
	ray->perp_wall_dist = orig_dist;
}

void	door_sliding(t_ray *ray, t_game *game, t_door *door)
{
	double	orig_dist;
	double	door_hit_pos;

	if (door->state == DOOR_OPEN)
		return ;
	if (ray->side == 0)
		orig_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		orig_dist = ray->side_dist.y - ray->delta_dist.y;
	get_hit_position(ray, game, orig_dist, &door_hit_pos);
	if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
	{
		process_door_animation(ray, door_hit_pos, door);
		return ;
	}
	ray->hit = true;
	ray->is_door = true;
	ray->perp_wall_dist = orig_dist;
}

void	adjust_door_texture(t_wall *wall)
{
	t_door	*door;
	double	animation_progress;
	int		offset;

	door = find_door(wall->game, wall->ray->map_x, wall->ray->map_y);
	if (!door || door->state == DOOR_OPEN)
		return ;
	if (door->state == DOOR_OPENING)
	{
		animation_progress = door->animation;
		offset = (int)(wall->texture->width * animation_progress);
		wall->tex.x = (int)((wall->tex.x - offset) + wall->texture->width)
			% wall->texture->width;
	}
	else if (door->state == DOOR_CLOSING)
	{
		animation_progress = 1.0 - door->animation;
		offset = (int)(wall->texture->width * animation_progress);
		wall->tex.x = (int)((wall->tex.x + offset) + wall->texture->width)
			% wall->texture->width;
	}
}
