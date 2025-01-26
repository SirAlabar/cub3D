/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_animation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/26 13:59:39 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	handle_firing_animation(t_game *game, double current_time)
{
	if (current_time - game->p1.last_fire <= 50)
		return ;
	game->p1.current_frame = (game->p1.current_frame + 1) % 4;
	game->p1.last_fire = current_time;
	if (game->p1.current_frame == 0)
		game->p1.is_firing = 0;
}

static void	handle_walking_animation(t_game *game, double current_time)
{
	if (current_time - game->p1.last_step <= 250)
		return ;
	if (game->p1.current_frame == 0)
		game->p1.current_frame = 1;
	else
		game->p1.current_frame = 0;
	game->p1.last_step = current_time;
}

static int	is_moving(t_keys keys)
{
	return (keys.w || keys.s || keys.a || keys.d);
}

void	update_weapon_animation(t_game *game)
{
	double	current_time;

	current_time = get_time_ms();
	if (game->p1.is_firing)
		handle_firing_animation(game, current_time);
	else if (is_moving(game->p1.keys))
		handle_walking_animation(game, current_time);
	else
		game->p1.current_frame = 0;
}
/*
void	process_door_texture(t_wall *wall, t_door *door, t_game *game)
{
	double	wallx;
	double	animation_progress;
	int		offset;

	wall->texture = &game->door_system->door_texture;
	if (wall->ray->side == 0)
		wallx = game->p1.pos.y + wall->ray->perp_wall_dist * wall->ray->dir.y;
	else
		wallx = game->p1.pos.x + wall->ray->perp_wall_dist * wall->ray->dir.x;
	wallx -= floor(wallx);
	wall->tex.x = (int)(wallx * wall->texture->width);
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
}*/
