/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 18:28:24 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <math.h>
# include <stdbool.h>
# include <sys/time.h>
# include <types.h>

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
    int   y_ceil[WINDOW_WIDTH];
    int   y_floor[WINDOW_WIDTH];
    int   y_top[WINDOW_WIDTH];
    int   y_bottom[WINDOW_WIDTH];
}               t_scanline;

typedef struct s_wall
{
    // Coordenadas da coluna na tela
    int             screen_x;    // Posição x na tela
    int             screen_y1;   // Topo da coluna na tela
    int             screen_y2;   // Base da coluna na tela
    
    // Dados de profundidade e escala
    t_fixed32       scale;      // Fator de escala da coluna
    t_fixed32       distance;   // Distância da coluna ao jogador
    
    // Propriedades da textura
    t_texture      *texture;    // Ponteiro para a textura
    t_fixed32       tex_u;      // Coordenada U da textura
    t_fixed32       tex_step;   // Passo da textura por pixel
    
    // Referências necessárias
    t_scanline     *buffer;     // Buffer de scanline
    t_game         *game;       // Referência ao estado do jogo
}                  t_wall;

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
void	draw_line_segment(t_game *game, t_bsp_line *line, t_scanline *buffer);
void draw_background(t_game *game);
void render_bsp_node_geometry(t_game *game, t_bsp_node *node, t_scanline *buffer);
void render_wall_segment(t_game *game, t_bsp_line *line, t_scanline *buffer);


// draw_room.c


// scanline_rendering.c
void					init_scanline_buffer(t_scanline *buffer);
void					set_wall_tex_coords(t_wall *wall);
void					init_wall_drawing(t_wall *wall);

// draw_weapon
void					draw_weapon(t_game *game);

/*
 * Texture Management
 */
// texture_manager.c
t_texture				*texture_create(t_game *game, char *texture_path);
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

void	init_double_buffer(t_game *game);
int	render_frame(t_game *game);
void swap_buffers(t_game *game);


t_fixed_vec32 transform_point(t_fixed_vec32 p, t_game *game);











#endif
