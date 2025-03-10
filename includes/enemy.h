/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:56:23 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:08:24 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

# define MIN_ENEMY_DISTANCE 1.5
# define ENEMY_ATTACK_DAMAGE 10
# define ENEMY_ATTACK_RANGE 2.0
# define ENEMY_ATTACK_COOLDOWN 1000
# define DAMAGE_FLASH_DURATION 200
# define DAMAGE_FLASH_COLOR 0xFF0000
# define DAMAGE_FLASH_ALPHA 0.4

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
	double		detection_radius;
	double		last_attack;
}				t_enemy;

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

void			init_enemies(t_game *game);
void			update_enemies(t_game *game);
void			draw_enemies(t_game *game);
void			add_enemy(t_game *game, t_vector pos);
void			draw_stripe_color(t_draw_params *p);
void			draw_enemy_stripe(t_game *game, t_enemy *enemy,
					t_sprite_data *s, int stripe);
void			draw_enemy_sprite(t_game *game, t_enemy *enemy,
					double screen_x, int sprite_height);
void			calculate_ray_step(t_ray_data *ray, t_vector ray_dir);
t_ray_data		init_ray_data(t_game *game, t_vector enemy_pos);
bool			is_enemy_visible(t_game *game, t_vector enemy_pos);
void			draw_enemy(t_game *game, t_enemy_list *current, double fov);
void			draw_enemies(t_game *game);
void			spawn_enemies_from_map(t_game *game);
void			init_enemies(t_game *game);
bool			c_enemy_mx(t_game *g, t_vector n_ps, t_vector dir, double padd);
bool			c_enemy_my(t_game *g, t_vector n_ps, t_vector dir, double padd);
void			shoot_enemy(t_game *game);
bool			is_in_bounds(t_game *game, int x, int y);
bool			has_hit_wall(t_game *game, t_ray_data *ray);

//add now
t_ray_data		init_ray_data(t_game *game, t_vector enemy_pos);
void			draw_enemy_stripe(t_game *game, t_enemy *enemy,
					t_sprite_data *s, int stripe);
void			calculate_ray_step(t_ray_data *ray, t_vector ray_dir);
void			draw_stripe_color(t_draw_params *p);
bool			enemy_can_attack(t_enemy *enemy, t_game *game);
void			enemy_attack_player(t_enemy *enemy, t_game *game);
void			process_enemy_attacks(t_game *game);
void			player_take_damage(t_game *game, int damage);

void			init_damage_feedback(t_game *game);
void			trigger_damage_effect(t_game *game);
void			update_damage_effect(t_game *game);
void			draw_damage_effect(t_game *game);

void			setup_draw_params(t_draw_params *p, t_enemy *enemy,
					t_sprite_data *s, int stripe);
void			draw_stripe_pixels(t_draw_params *p, t_sprite_data *s);
void			draw_enemy_stripe(t_game *game, t_enemy *enemy,
					t_sprite_data *s, int stripe);

void			process_active_enemy(t_enemy_list *current, t_game *game,
					double speed);
void			calculate_enemy_distance(t_game *game, t_enemy *enemy);
t_vector		try_move_x(t_enemy *enemy, t_game *game,
					double speed);
t_vector		try_move_y(t_enemy *enemy, t_game *game,
					double speed);
void			update_enemy_position(t_enemy *enemy, t_game *g, double speed);

void			init_damage_feedback(t_game *game);
void			trigger_damage_effect(t_game *game);
void			update_damage_effect(t_game *game);
double			calculate_flash_intensity(t_game *game);
unsigned int	blend_color(unsigned int original, double intensity);

bool			check_short_distance(t_game *game, t_vector enemy_pos);
bool			ray_trace_to_enemy(t_game *game, t_ray_data *ray,
					t_vector enemy_pos, int max_steps);
bool			is_enemy_visible(t_game *game, t_vector enemy_pos);
void			adjust_angle_and_fov(double *angle, double *effective_fov,
					double fov, double dist);
int				calculate_sprite_height(double dist);
void			draw_enemy(t_game *game, t_enemy_list *current, double fov);
void			check_and_draw_enemy(t_game *g, t_enemy_list *crnt, double fov);

#endif
