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
    
    // Se não há movimento, retorna true
    if (new_x == player->pos.x && new_y == player->pos.y)
        return (true);
    
    new_pos.x = new_x;
    new_pos.y = new_y;
    start_pos = player->pos;
    
/*   ft_printf(BLUE"Checking move:\n"DEFAULT);
    ft_printf("From: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(start_pos.x), fixed32_to_int(start_pos.y),
        start_pos.x, start_pos.y);
    ft_printf("To: (%d,%d) [fixed: (%d,%d)]\n",
        fixed32_to_int(new_pos.x), fixed32_to_int(new_pos.y),
        new_pos.x, new_pos.y);*/
        
    return check_movement_valid(game->bsp_tree, start_pos, new_pos);
}
/*
** Attempts to move player along a wall when direct movement is blocked
** Uses separate X and Y movement checks to find valid sliding direction
*/
void slide_along_wall(t_game *game, t_fixed32 newx, t_fixed32 newy)
{
    bool moved;
    
    // Se não há movimento proposto, retorna
    if (newx == game->p1.pos.x && newy == game->p1.pos.y)
        return;
    
  //  ft_printf(YELLOW"Attempting to slide along wall\n"DEFAULT);
    moved = false;
    
    // Tenta mover só no eixo X
    if (newx != game->p1.pos.x)
    {
        if (check_sector_move(game, &game->p1, newx, game->p1.pos.y))
        {
          //  ft_printf(GREEN"Sliding along X axis\n"DEFAULT);
            game->p1.pos.x = newx;
            game->p1.momy = 0;
            moved = true;
        }
    }
    
    // Tenta mover só no eixo Y se não moveu em X
    if (!moved && newy != game->p1.pos.y)
    {
        if (check_sector_move(game, &game->p1, game->p1.pos.x, newy))
        {
           // ft_printf(GREEN"Sliding along Y axis\n"DEFAULT);
            game->p1.pos.y = newy;
            game->p1.momx = 0;
            moved = true;
        }
    }
    
    // Se não conseguiu mover em nenhum eixo, zera o momentum
    if (!moved)
    {
      //  ft_printf(RED"Could not slide - zeroing momentum\n"DEFAULT);
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
    bool has_movement;

    build_player_cmd(&game->p1);

    // Se não há input, aplica fricção
    if (game->p1.cmd.forward == 0 && game->p1.cmd.side == 0)
    {
        apply_friction(&game->p1);
    }
    else 
    {
        // Calcula o movimento frontal
        if (game->p1.cmd.forward)
        {
            angle = game->p1.angle >> ANGLETOFINESHIFT;
            t_fixed32 cos_val = get_cos_8192(game->fixed_tables, angle);
            t_fixed32 sin_val = get_sin_8192(game->fixed_tables, angle);
            
            game->p1.momx = fixed32_mul(game->p1.cmd.forward, cos_val);
            game->p1.momy = fixed32_mul(game->p1.cmd.forward, sin_val);
        }
        
        // Calcula o movimento lateral
        if (game->p1.cmd.side)
        {
            angle = (game->p1.angle - ANG90) >> ANGLETOFINESHIFT;
            t_fixed32 cos_val = get_cos_8192(game->fixed_tables, angle);
            t_fixed32 sin_val = get_sin_8192(game->fixed_tables, angle);
            
            // Adiciona o movimento lateral ao momentum existente
            game->p1.momx = fixed32_add(game->p1.momx, 
                fixed32_mul(game->p1.cmd.side, cos_val));
            game->p1.momy = fixed32_add(game->p1.momy, 
                fixed32_mul(game->p1.cmd.side, sin_val));
        }
    }

    // Aplica limite de velocidade se necessário
    if (game->p1.momx != 0 || game->p1.momy != 0)
    {
        t_fixed32 speed_sq = fixed32_add(
            fixed32_mul(game->p1.momx, game->p1.momx),
            fixed32_mul(game->p1.momy, game->p1.momy)
        );

        if (speed_sq > fixed32_mul(MAXMOVE, MAXMOVE))
        {
            t_fixed32 scale = fixed32_div(MAXMOVE, fixed32_sqrt(speed_sq));
            game->p1.momx = fixed32_mul(game->p1.momx, scale);
            game->p1.momy = fixed32_mul(game->p1.momy, scale);
        }
    }

    // Aplica o movimento apenas se houver momentum
    has_movement = (game->p1.momx != 0 || game->p1.momy != 0);
    if (has_movement)
    {
        new_pos.x = fixed32_add(game->p1.pos.x, game->p1.momx);
        new_pos.y = fixed32_add(game->p1.pos.y, game->p1.momy);

        if (check_sector_move(game, &game->p1, new_pos.x, new_pos.y))
        {
            game->p1.pos = new_pos;
        }
        else
        {
            slide_along_wall(game, new_pos.x, new_pos.y);
        }
    }
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
