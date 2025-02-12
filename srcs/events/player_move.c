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

static bool check_sector_move(t_game *game, t_player *player, t_fixed32 new_x, t_fixed32 new_y)
{
    t_fixed_vec32 new_pos;
    t_fixed_vec32 start_pos;
    new_pos.x = new_x;
    new_pos.y = new_y;
    start_pos = player->pos;
    return check_movement_valid(game->bsp_tree, start_pos, new_pos);
}

/*
** Attempts to move player along a wall when direct movement is blocked
** Uses separate X and Y movement checks to find valid sliding direction
*/
void slide_along_wall(t_game *game, t_fixed32 newx, t_fixed32 newy)
{
    t_fixed_vec32 new_pos;
    new_pos.x = newx;
    new_pos.y = newy;
    
    if (check_sector_move(game, &game->p1, new_pos.x, game->p1.pos.y))
    {
        game->p1.pos.x = new_pos.x;
        game->p1.momy = 0;
    }
    else if (check_sector_move(game, &game->p1, game->p1.pos.x, new_pos.y))
    {
        game->p1.pos.y = new_pos.y;
        game->p1.momx = 0;
    }
    else
    {
        game->p1.momx = 0;
        game->p1.momy = 0;
    }
}

/*
** Updates player position based on current momentum and angle
** Handles collision detection and sliding movement when blocked
*/
void move_player(t_game *game)
{
    t_fixed_vec32 new_pos;
    t_fixed32 angle;

    build_player_cmd(&game->p1);

    if (game->p1.cmd.forward)
    {
        angle = game->p1.angle >> ANGLETOFINESHIFT;
        game->p1.momx += fixed32_mul(game->p1.cmd.forward, 
            get_cos_8192(game->fixed_tables, angle));
        game->p1.momy += fixed32_mul(game->p1.cmd.forward, 
            get_sin_8192(game->fixed_tables, angle));
    }
    
    if (game->p1.cmd.side)
    {
        angle = (game->p1.angle - ANG90) >> ANGLETOFINESHIFT;
        game->p1.momx += fixed32_mul(game->p1.cmd.side, 
            get_cos_8192(game->fixed_tables, angle));
        game->p1.momy += fixed32_mul(game->p1.cmd.side, 
            get_sin_8192(game->fixed_tables, angle));
    }

    apply_momentum(&game->p1);
    apply_friction(&game->p1);
    limit_momentum(&game->p1);
    
    new_pos.x = game->p1.pos.x + game->p1.momx;
    new_pos.y = game->p1.pos.y + game->p1.momy;

    if (check_movement_valid(game->bsp_tree, game->p1.pos, new_pos))
    {
        game->p1.pos = new_pos;
    }
    else
        slide_along_wall(game, new_pos.x, new_pos.y);
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
