/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_gun.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:47:39 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:47:40 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	process_firing_animation(t_portal_gun *gun, int *frame_counter,
		double current_time)
{
	if (current_time - gun->last_fire_time <= 60)
		return ;
	(*frame_counter)++;
	if (*frame_counter >= 3)
	{
		gun->current_frame = (gun->current_frame + 1) % 4;
		*frame_counter = 0;
		if (gun->current_frame == 0)
			gun->is_firing = 0;
	}
	gun->last_fire_time = current_time;
}

void	update_portal_gun_animation(t_game *game)
{
	double			current_time;
	t_portal_gun	*gun;
	static int		frame_counter = 0;

	gun = &game->portal_system->gun;
	current_time = get_time_ms();
	if (gun->is_firing)
		process_firing_animation(gun, &frame_counter, current_time);
	else
	{
		gun->current_frame = 0;
		frame_counter = 0;
	}
}

static int	check_portal_cooldown(t_game *game)
{
	double	current_time;

	if (game->active_weapon != 1)
		return (0);
	current_time = get_time_ms();
	if (current_time - game->portal_system->gun.last_fire_time
		< game->portal_system->gun.cooldown)
		return (0);
	game->portal_system->gun.is_firing = 1;
	game->portal_system->gun.current_frame = 1;
	return (1);
}

static int	can_create_portal(t_game *game, t_portal_hit hit,
		t_portal_wall *other_portal, t_cardinal hit_cardinal)
{
	if (!hit.found)
		return (0);
	if (!is_valid_portal_surface(game, hit))
		return (0);
	if (other_portal->active && other_portal->position.x == hit.wall_pos.x
		&& other_portal->position.y == hit.wall_pos.y
		&& (hit_cardinal == other_portal->card
			|| hit_cardinal == get_opposite_cardinal(other_portal->card)))
		return (0);
	return (1);
}

void	fire_portal_gun(t_game *game)
{
	t_portal_hit	hit;
	t_portal_type	portal_type;
	t_portal_wall	*other_portal;
	t_cardinal		hit_cardinal;
	double			current_time;

	if (!check_portal_cooldown(game))
		return ;
	play_game_sound(game, SOUND_PORTAL);
	portal_type = game->portal_system->gun.active_portal;
	hit = detect_wall_for_portal(game);
	hit_cardinal = get_cardinal_from_normal(hit.normal);
	if (portal_type == PORTAL_BLUE)
		other_portal = &game->portal_system->orange_portal;
	else
		other_portal = &game->portal_system->blue_portal;
	if (!can_create_portal(game, hit, other_portal, hit_cardinal))
		return ;
	create_portal(game, hit, portal_type);
	current_time = get_time_ms();
	game->portal_system->gun.last_fire_time = current_time;
}
