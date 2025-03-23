/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:11:37 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:11:54 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	handle_firing(t_game *game)
{
	game->p1.is_firing = 1;
	shoot_enemy(game);
	if (game->sounds && game->sounds->is_initialized)
		play_sound(game->sounds->gun);
}

void	switch_weapon(t_game *game)
{
	if (game->active_weapon == 0)
	{
		game->active_weapon = 1;
	}
	else
	{
		game->active_weapon = 0;
	}
	game->p1.is_firing = 0;
	game->p1.current_frame = 0;
	if (game->portal_system)
	{
		game->portal_system->gun.is_firing = 0;
		game->portal_system->gun.current_frame = 0;
	}
}
