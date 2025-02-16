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

void apply_friction(t_player *player)
{
    ft_printf(CYAN"Applying friction:\n"DEFAULT);
    ft_printf("Current momentum: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(player->momx), fixed32_to_int(player->momy),
        player->momx, player->momy);

    // Se há input ativo, não aplica fricção
    if (player->cmd.forward || player->cmd.side)
    {
        ft_printf(YELLOW"Movement input detected - skipping friction\n"DEFAULT);
        return;
    }

    // Calcula velocidade total
    t_fixed32 speed_sq = fixed32_add(
        fixed32_mul(player->momx, player->momx),
        fixed32_mul(player->momy, player->momy)
    );

    // Se a velocidade é muito baixa, para completamente
    if (speed_sq < STOPSPEED)
    {
        ft_printf(GREEN"Below stop speed - zeroing momentum\n"DEFAULT);
        player->momx = 0;
        player->momy = 0;
        return;
    }

    // Se a velocidade está entre STOPSPEED e 2*STOPSPEED, aplica mais fricção
    if (speed_sq < (STOPSPEED << 1))
    {
        player->momx = fixed32_mul(player->momx, FRICTION >> 1);
        player->momy = fixed32_mul(player->momy, FRICTION >> 1);
    }
    else
    {
        player->momx = fixed32_mul(player->momx, FRICTION);
        player->momy = fixed32_mul(player->momy, FRICTION);
    }
    
    ft_printf("After friction: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(player->momx), fixed32_to_int(player->momy),
        player->momx, player->momy);
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
void limit_momentum(t_player *player)
{
    t_fixed32 speed_sq;
    t_fixed32 scale;
    
    ft_printf("Limiting momentum:\n");
    ft_printf("Before limit: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(player->momx), fixed32_to_int(player->momy),
        player->momx, player->momy);

    // Calcula velocidade total
    speed_sq = fixed32_add(
        fixed32_mul(player->momx, player->momx),
        fixed32_mul(player->momy, player->momy)
    );

    // Se excede a velocidade máxima, escala proporcionalmente
    if (speed_sq > fixed32_mul(MAXMOVE, MAXMOVE))
    {
        scale = fixed32_div(MAXMOVE, fixed32_sqrt(speed_sq));
        player->momx = fixed32_mul(player->momx, scale);
        player->momy = fixed32_mul(player->momy, scale);
    }

    ft_printf("After limit: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(player->momx), fixed32_to_int(player->momy),
        player->momx, player->momy);
}