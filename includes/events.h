/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:11 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/14 19:19:11 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

# include <types.h>

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}		t_keys;

typedef struct s_cmd
{
        t_fixed32 forward;
        t_fixed32 side;
        t_fixed32 turn;
        int     attack;
        int     use;
        int     jump;        
}		t_cmd;

/*
 * Key Event Functions
 */
void	handle_movement(t_game *game);
void	build_player_cmd(t_player *player);
int handle_key_press(int keycode, t_game *game);
int handle_key_release(int keycode, t_game *game);
void	player_jump(t_game *game);
void	player_fall(t_game *game);


/*
 * Player Movement Functions
 */
void	move_player(t_game *game);

void	apply_momentum(t_player *player);
void	apply_friction(t_player *player);
void	apply_gravity(t_player *player);
void	limit_momentum(t_player *player);

/*
 * Mouse Event Functions
 */
int		mouse_move(t_game *game, t_vector pos);
int		mouse_wrapper(int x, int y, void *param);
int	handle_mouse_move(int x, int y, t_game *game);

/*
 * Utility Functions
 */
t_fixed32 increment_angle(t_fixed32 current_angle, t_fixed32 increment);
t_fixed32 wrap_angle(t_fixed32 angle);

#endif