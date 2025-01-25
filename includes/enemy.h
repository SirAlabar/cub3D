/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:56:23 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/22 17:58:04 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H
#include <stdbool.h>

typedef struct s_sprite_data
{
	int	   height;
	int	   width;
	double	screen_x;
	int	   start_y;
	int	   end_y;
	int	   start_x;
	int	   end_x;
}    t_sprite_data;

typedef struct s_ray_data
{
	t_vector	pos;
	t_vector	delta_dist;
	t_vector	side_dist;
	t_vector	step;
	int		   map_x;
	int		   map_y;
}    t_ray_data;

typedef struct	s_enemy
{
	t_vector	pos;
	t_vector	dir;
	int			health;
	bool		alive;
	double		dist_to_player;
	t_texture	*texture;
}				t_enemy;

typedef struct	s_enemy_list
{
	t_enemy				enemy;
	struct s_enemy_list	*next;
}				t_enemy_list;

double	vector_length(t_vector v);
void    init_enemies(t_game *game);
void    update_enemies(t_game *game);
void    draw_enemies(t_game *game);
void    add_enemy(t_game *game, t_vector pos);
#endif
