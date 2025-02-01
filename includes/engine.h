/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 19:50:21 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <cub3d.h>
# include <math.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_game	t_game;

typedef struct s_vector_i
{
	int					x;
	int					y;
}						t_vector_i;

typedef struct s_vector
{
	double				x;
	double				y;
}						t_vector;

typedef struct s_texture
{
	void				*img;
	char				*addr;
	int					width;
	int					height;
	int					bpp;
	int					line_len;
	int					endian;
	char				*path;
}						t_texture;

typedef struct s_scanline
{
	int					y_top[WINDOW_WIDTH];
	int					y_bottom[WINDOW_WIDTH];
}						t_scanline;

typedef struct s_line
{
	int					x;
	int					start;
	int					end;
}						t_line;

typedef struct s_ray
{
	t_vector			dir;
	t_vector			side_dist;
	t_vector			delta_dist;
	double				perp_wall_dist;
	double				step;
	int					tex_x;
	int					line_height;
	int					map_x;
	int					map_y;
	int					step_x;
	int					step_y;
	bool				hit;
	int					side;
	int					draw_start;
	int					draw_end;
}						t_ray;

typedef struct s_wall
{
	t_vector			pos;
	t_vector			tex;
	t_vector_i			screen;
	double				step;
	double				tex_pos;
	double				perp_wall_dist;
	int					height;
	int					start;
	int					end;
	int					color;
	t_texture			*texture;
	t_game				*game;
	t_ray				*ray;
	t_scanline			*buffer;
	int					x;
}						t_wall;

/*
 * Core Engine Functions
 */
// engine_render.c
void					engine_prepare_frame(t_game *game);
int						engine_render_frame(t_game *game);

double					get_time_ms(void);
void					update_fps(t_game *game);

/*
 * Drawing Functions
 */
// draw_basic.c
void					draw_pixel(t_game *game, int x, int y, int color);
void					draw_texture_pixel(t_texture *tex, int x, int y,
							int color);
unsigned int			get_texture_pixel(t_texture *tex, int x, int y);
unsigned int			apply_shade(unsigned int color, double shade);

// draw_room.c
int						draw_background(t_game *game);
void					draw_wall(t_game *game, t_ray *ray, int x);
void					update_ray_position(t_ray *ray);

// scanline_rendering.c
void					init_scanline_buffer(t_scanline *buffer);
void					draw_vertical_line(t_game *g, t_line line, int color);
void					set_wall_tex_coords(t_wall *wall);
void					draw_wall_scanline(t_game *game, t_ray *ray, int x,
							t_scanline *buffer);
void					init_wall_drawing(t_wall *wall);

// draw_weapon
void					draw_weapon(t_game *game);

/*
 * Texture Management
 */
// texture_manager.c
t_texture				*texture_create(t_game *game, char *texture_path);
t_texture				*get_wall_texture(t_ray *ray, t_game *game);
void					texture_destroy(t_texture **texture, void *mlx);
void					update_weapon_animation(t_game *game);
void					resize_texture(t_texture *src, t_texture *dst);

/*
 * Vector Operations
 */
// Constructor
t_vector				vector_create(double x, double y);

// Basic operations
t_vector				vector_add(t_vector v1, t_vector v2);
t_vector				vector_sub(t_vector v1, t_vector v2);
t_vector				vector_mult(t_vector v, double n);
t_vector				vector_div(t_vector v, double n);

// Vector operations
double					vector_dot(t_vector v1, t_vector v2);
double					vector_mag(t_vector v);
double					vector_mag_sq(t_vector v);
t_vector				vector_normalize(t_vector v);

// Vector I operations
t_vector_i				vector_i_create(int x, int y);
bool					vector_i_equals(t_vector_i a, t_vector_i b);
t_vector_i				vector_to_vector_i(t_vector v);
t_vector				vector_i_to_vector(t_vector_i v);
double					vector_i_dist(t_vector_i v1, t_vector_i v2);

// Angular operations
double					vector_heading(t_vector v);
t_vector				vector_rotate(t_vector v, double angle);
t_vector				vector_lerp(t_vector start, t_vector end, double amt);
double					vector_dist(t_vector v1, t_vector v2);
t_vector				vector_limit(t_vector v, double max);

/*
 * Raycasting
 */
// raycast_dda.c
void					init_ray(t_ray *ray, t_game *game, int x);
void					step_side_dist(t_ray *ray, t_game *game);
void					perform_dda(t_ray *ray, t_game *game);
void					wall_height(t_ray *ray);
void					cast_rays(t_game *game, t_ray *rays);
void					check_collisions(t_ray *ray, t_game *game);
void					handle_wall_collision(t_ray *ray);
void					handle_door_collision(t_ray *ray, t_game *game);
double					get_wall_x(t_ray *ray, t_game *game, double orig_dist);
void					set_door_hit(t_ray *ray, double orig_dist);

#endif
