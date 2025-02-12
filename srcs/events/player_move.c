/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 15:09:43 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Attempts to move player along a wall when direct movement is blocked
** Uses separate X and Y movement checks to find valid sliding direction
*/
static void	slide_along_wall(t_game *game, fixed_t newx, fixed_t newy)
{
	t_player	*p;

	p = &game->p1;
	if (check_sector_move(p, newx, p->pos.y))
	{
		p->pos.x = newx;
		p->momy = 0;
	}
	else if (check_sector_move(p, p->pos.x, newy))
	{
		p->pos.y = newy;
		p->momx = 0;
	}
	else
	{
		p->momx = 0;
		p->momy = 0;
	}
}

/*
** Updates player position based on current momentum and angle
** Handles collision detection and sliding movement when blocked
*/
void	move_player(t_game *game)
{
	t_player	*p;
	fixed_t		newx;
	fixed_t		newy;
	angle_t		angle;

	p = &game->p1;
	build_player_cmd(p);
	if (p->cmd.forward)
	{
		angle = p->angle >> ANGLETOFINESHIFT;
		p->momx += fixed_mul(p->cmd.forward, finecosine[angle & FINEMASK]);
		p->momy += fixed_mul(p->cmd.forward, finesine[angle & FINEMASK]);
	}
	if (p->cmd.side)
	{
		angle = (p->angle - ANG90) >> ANGLETOFINESHIFT;
		p->momx += fixed_mul(p->cmd.side, finecosine[angle & FINEMASK]);
		p->momy += fixed_mul(p->cmd.side, finesine[angle & FINEMASK]);
	}
	apply_momentum(p);
	apply_friction(p);
	limit_momentum(p);
	newx = p->pos.x + p->momx;
	newy = p->pos.y + p->momy;
	if (check_sector_move(p, newx, newy))
	{
		p->pos.x = newx;
		p->pos.y = newy;
		update_player_sector(game);
	}
	else
		slide_along_wall(game, newx, newy);
}

/*
** Initiates player jump if currently on ground
** Sets vertical momentum and updates ground contact state
*/
void	player_jump(t_game *game)
{
	t_player	*p;

	p = &game->p1;
	if (p->on_ground)
	{
		p->momz = JUMPSPEED;
		p->on_ground = 0;
	}
}

/*
** Handles vertical movement including gravity and collision with floor/ceiling
** Updates player's Z position and ground contact state
*/
void	player_fall(t_game *game)
{
	t_player	*p;
	t_sector	*sector;

	p = &game->p1;
	sector = p->sector;
	if (sector == NULL)
		return ;
	apply_gravity(p);
	p->z += p->momz;
	if (p->z <= sector->floor_height)
	{
		p->z = sector->floor_height;
		p->momz = 0;
		p->on_ground = 1;
	}
	if (p->z + PLAYER_HEIGHT > sector->ceiling_height)
	{
		p->z = sector->ceiling_height - PLAYER_HEIGHT;
		p->momz = 0;
	}
	p->view_z = p->z + PLAYER_VIEW_HEIGHT;
}
