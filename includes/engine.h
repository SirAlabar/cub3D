/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:28:20 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/06 18:15:22 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <cub3d.h>
# include <stdbool.h>
# include <math.h>

typedef struct s_game	t_game;

typedef struct s_vector
{
	double				x;
	double				y;
}						t_vector;

typedef struct s_ray 
{
	t_vector dir;
	t_vector side_dist;
	t_vector delta_dist;
	double perp_wall_dist;
	int line_height;
	int map_x;
	int map_y;
	int step_x;
	int step_y;
	bool hit;
	int side;
	int draw_start;
	int draw_end;
}	t_ray;

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

/*
 * Core Engine Functions
 */
// engine_render.c
void					engine_prepare_frame(t_game *game);
void					engine_render_frame(t_game *game);

/*
 * Drawing Functions
 */
// draw_basic.c
void					draw_pixel(t_game *game, int x, int y, int color);
void					draw_texture_pixel(t_texture *tex, int x, int y,
							int color);
unsigned int			get_texture_pixel(t_texture *tex, int x, int y);

// draw_background.c
int						draw_background(t_game *game);
void draw_wall(t_game *game, t_ray *ray, int x);

/*
 * Texture Management
 */
// texture_manager.c
t_texture				*texture_create(t_game *game, char *texture_path);
void					texture_destroy(t_texture **texture, void *mlx);

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
void init_ray(t_ray *ray, t_game *game, int x);
void step_side_dist(t_ray *ray, t_game *game);
void perform_dda(t_ray *ray, t_game *game);
void wall_height(t_ray *ray);
void cast_rays(t_game *game);


// raycast_walls.c


/*
// draw_basic.c
void					draw_pixel(t_game *game, int x, int y, int color);
void	draw_line(t_game *game, t_vector start, t_vector end,
							int color);
void	draw_rect(t_game *game, t_vector pos, t_vector size,
							int color);

// draw_background.c
void					draw_floor(t_game *game);
void					draw_ceiling(t_game *game);
void					draw_background(t_game *game);
*/
// draw_player.c
// void        draw_player(t_game *game);
// void        draw_player_direction(t_game *game);

// draw_minimap.c
// void        draw_minimap(t_game *game);
// void        draw_minimap_walls(t_game *game);
// void        draw_minimap_player(t_game *game);

#endif