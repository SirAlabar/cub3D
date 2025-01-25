/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 20:37:09 by hluiz-ma         ###   ########.fr       */
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

	ds = game->door_system;
	delta_time = 1.0 / 60.0;
	i = 0;
	while (i < ds->door_count)
	{
		update_door_state(&ds->doors[i], delta_time);
		i++;
	}
}
/*
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
}*/
/*
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
}*/

void	door_sliding(t_ray *ray, t_game *game, t_door *door)
{
	double	orig_dist;
	//double	door_hit_pos;
	double	wallx;

	// Se a porta já está aberta, não precisa realizar mais cálculos
	if (door->state == DOOR_OPEN)
		return ;

	// Calcular a distância original e ajustar para renderizar no meio do tile
	if (ray->side == 0)
	{
		ray->side_dist.x -= ray->delta_dist.x / 2; // Ajuste para meio do tile
		if (ray->side_dist.x > ray->side_dist.y)
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
			orig_dist = ray->side_dist.y - ray->delta_dist.y;
		}
		else
			orig_dist = ray->side_dist.x - ray->delta_dist.x;
		ray->side_dist.x += ray->delta_dist.x;
	}
	else
	{
		ray->side_dist.y -= ray->delta_dist.y / 2; // Ajuste para meio do tile
		if (ray->side_dist.y > ray->side_dist.x)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
			orig_dist = ray->side_dist.x - ray->delta_dist.x;
		}
		else
			orig_dist = ray->side_dist.y - ray->delta_dist.y;
		ray->side_dist.y += ray->delta_dist.y;
	}

	// Marcar o impacto com a porta
	ray->hit = true;
	ray->is_door = true;
	ray->perp_wall_dist = orig_dist;

	// Calcular a posição de impacto da porta no eixo horizontal
	wallx = get_wall_x(ray, game, orig_dist);
	
	// Ajuste para garantir que as molduras laterais da porta não saiam
	if (wallx > door->animation && wallx < 1.0)
	{
		set_door_hit(ray, orig_dist);
	}


}