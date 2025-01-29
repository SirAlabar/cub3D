/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:56:23 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/29 17:44:56 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

typedef struct s_sprite_data
{
	int		height;
	int		width;
	double	screen_x;
	int		start_y;
	int		end_y;
	int		start_x;
	int		end_x;
}				t_sprite_data;

typedef struct s_ray_data
{
	t_vector	pos;
	t_vector	delta_dist;
	t_vector	side_dist;
	t_vector	step;
	int			map_x;
	int			map_y;
}				t_ray_data;

typedef struct s_enemy
{
	t_vector	pos;
	t_vector	dir;
	int			health;
	bool		alive;
	double		dist_to_player;
	t_texture	*texture;
	double		detection_radius; // Add this line
} t_enemy;

typedef struct s_draw_params
{
	t_game		*game;
	t_enemy		*enemy;
	t_vector	pos;
	t_vector	tex;
}	t_draw_params;

typedef struct s_enemy_list
{
	t_enemy				enemy;
	struct s_enemy_list	*next;
}				t_enemy_list;

double		vector_length(t_vector v);
void		init_enemies(t_game *game);
void		update_enemies(t_game *game);
void		draw_enemies(t_game *game);
void		add_enemy(t_game *game, t_vector pos);
void		draw_stripe_color(t_draw_params *p);
void		draw_enemy_stripe(t_game *game, t_enemy *enemy,
				t_sprite_data *s, int stripe);
void		draw_enemy_sprite(t_game *game, t_enemy *enemy,
				double screen_x, int sprite_height);
void		calculate_ray_step(t_ray_data *ray, t_vector ray_dir);
t_ray_data	init_ray_data(t_game *game, t_vector enemy_pos);
bool		is_enemy_visible(t_game *game, t_vector enemy_pos);
void		draw_enemy(t_game *game, t_enemy_list *current, double fov);
void		draw_enemies(t_game *game);
void		spawn_enemies_from_map(t_game *game);
void		init_enemies(t_game *game);
bool		c_enemy_mx(t_game *game, t_vector n_pos, t_vector dir, double padd);
bool		c_enemy_my(t_game *game, t_vector n_pos, t_vector dir, double padd);
void		shoot_enemy(t_game *game);
bool	is_in_bounds(t_game *game, int x, int y);
bool	has_hit_wall(t_game *game, t_ray_data *ray);

#endif
