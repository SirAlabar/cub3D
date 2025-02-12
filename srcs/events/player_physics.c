/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_physics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 15:09:48 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Applies movement momentum based on player input
** Scales movement vectors using maximum allowed movement speed
*/
void	apply_momentum(t_player *player)
{
	if (player->cmd.forward == 0 && player->cmd.side == 0)
		return ;
	player->momx = fixed32_mul(player->momx, MAXMOVE);
	player->momy = fixed32_mul(player->momy, MAXMOVE);
}

/*
** Applies friction to player movement when not actively moving
** Gradually reduces speed or stops completely when below threshold
*/
void	apply_friction(t_player *player)
{
	int	below_stop_speed;

	if (player->cmd.forward || player->cmd.side)
		return ;
	below_stop_speed = abs(player->momx) < STOPSPEED && 
		abs(player->momy) < STOPSPEED;
	if (below_stop_speed)
	{
		player->momx = 0;
		player->momy = 0;
		return ;
	}
	player->momx = fixed32_mul(player->momx, FRICTION);
	player->momy = fixed32_mul(player->momy, FRICTION);
}

/*
** Applies gravitational force to player when not on ground
** Reduces vertical momentum by gravity constant
*/
void	apply_gravity(t_player *player)
{
	if (player->on_ground == 0)
		player->momz -= GRAVITY;
}

/*
** Enforces maximum momentum limits for player movement
** Caps horizontal and vertical speeds at defined thresholds
*/
void	limit_momentum(t_player *player)
{
	if (player->momx > MAXMOVE)
		player->momx = MAXMOVE;
	if (player->momx < -MAXMOVE)
		player->momx = -MAXMOVE;
	if (player->momy > MAXMOVE)
		player->momy = MAXMOVE;
	if (player->momy < -MAXMOVE)
		player->momy = -MAXMOVE;
	if (player->momz < -MAXMOVE)
		player->momz = -MAXMOVE;
}
