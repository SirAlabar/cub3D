/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:11 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:49:11 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

# include <cub3d.h>

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
	int	shift;	
}		t_keys;

/*
 * Key Event Functions
 */
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
void	handle_movement(t_game *game);
int		menu_key_press(int keycode, t_game *game);

/*
 * Player Movement Functions
 */
void	move_player(t_game *game, double dir_x, double dir_y);
void	rotate_player(t_game *game, double angle);
bool	can_move_x(t_game *game, t_vector new, t_vector dir, double buff);
bool	can_move_y(t_game *game, t_vector new, t_vector dir, double buff);
bool	check_wall_collision(t_game *g, int x, int y);

/*
 * Mouse Event Functions
 */
int		mouse_move(t_game *game, t_vector pos);
int		mouse_wrapper(int x, int y, void *param);

/*
 * Door Event Functions
 */
void	render_door(t_game *game, t_ray *ray, int x);
void	render_door_line(t_game *game, t_ray *ray, t_door *door, int x);
t_door	*find_door(t_game *game, int x, int y);
void	interact_with_door(t_game *game);
void	add_door(t_game *game, int x, int y);
void	init_door_system(t_game *game);
void	init_door(t_door *door, int x, int y, t_door_orientation orient);

/*
 * Utility Functions
 */
void	*keys_else(t_game *game, double *dir_x, double *dir_y);
void	switch_weapon(t_game *game);

#endif