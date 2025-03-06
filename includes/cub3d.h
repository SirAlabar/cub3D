/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:34 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:48:12 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* Window settings */
# define WINDOW_WIDTH 1200
# define WINDOW_HEIGHT 900
# define FOV 60
# define MOVE_SPEED 0.02
# define ROTATION_SPEED 0.04
# define MAX_ROTATION 0.045

# include <colors.h>
# include <engine.h>
# include <events.h>
# include <portals.h>
# include <minimap.h>
# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <time.h>
# include <mlx_utils.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <validations.h>
# include <checker.h>
# include <health.h>
# include <enemy.h>
/*
# ifdef MAC_OS
#  include <mlx.h>
#  define KEY_ESC 53
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_E 14
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_UP 126
#  define KEY_DOWN 125
# else*/
# include <X11/X.h>
# include <X11/keysym.h>
# include <mlx.h>
# define KEY_ESC XK_Escape
# define KEY_W XK_w
# define KEY_A XK_a
# define KEY_S XK_s
# define KEY_D XK_d
# define KEY_E XK_e
# define KEY_Q XK_q
# define KEY_LEFT XK_Left
# define KEY_RIGHT XK_Right
# define KEY_UP XK_Up
# define KEY_DOWN XK_Down
# define KEY_SPACE XK_space
# define MOUSE_LEFT 1
# define MOUSE_RIGHT 3
//# endif

/* Map characters */
# define VALID_MAP_CHARS "01NSEWDM "
# define PLAYER_CHARS "NSEW"

/* Error messages */
# define ERR_ARGS "Error\nInvalid number of arguments\n"
# define ERR_FILE "Error\nInvalid file extension or unable to open file\n"
# define ERR_MAP "Error\nInvalid map configuration\n"
# define ERR_TEXTURE "Error\nInvalid texture configuration\n"
# define ERR_COLOR "Error\nInvalid color configuration\n"
# define ERR_MALLOC "Error\nMemory allocation failed\n"

/* Gun Frames*/
# define GUN_F1 "./assets/sprites/pistol/PIS0.xpm"
# define GUN_F2 "./assets/sprites/pistol/PIS1.xpm"
# define GUN_F3 "./assets/sprites/pistol/PIS2.xpm"
# define GUN_F4 "./assets/sprites/pistol/PISFA0.xpm"

/* Skyboxs*/
# define SKYBOX1 "./assets/texture/skybox1.xpm"
# define SKYBOX2 "./assets/texture/skybox2.xpm"
# define SKYBOX3 "./assets/texture/skybox3.xpm"
# define SKYBOX4 "./assets/texture/skybox4.xpm"
# define SKYBOX5 "./assets/texture/skybox5.xpm"
# define SKYBOX6 "./assets/texture/skybox6.xpm"
# define SKYBOX7 "./assets/texture/skybox7.xpm"
# define SKYBOX8 "./assets/texture/skybox8.xpm"
# define SKYBOX9 "./assets/texture/skybox9.xpm"
# define SKYBOX10 "./assets/texture/skybox10.xpm"

/* Floor Textures */
# define FLOOR1 "./assets/texture/tile_floor1.xpm"
# define FLOOR2 "./assets/texture/tile_floor2.xpm"
# define FLOOR3 "./assets/texture/tile_floor3.xpm"
# define FLOOR4 "./assets/texture/tile_floor4.xpm"
# define FLOOR5 "./assets/texture/tile_floor5.xpm"
# define FLOOR6 "./assets/texture/tile_floor6.xpm"
# define FLOOR7 "./assets/texture/tile_floor7.xpm"
# define FLOOR8 "./assets/texture/tile_floor8.xpm"
# define FLOOR9 "./assets/texture/tile_floor9.xpm"
# define FLOOR10 "./assets/texture/tile_floor10.xpm"

/* Structs */

typedef struct s_player
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
	double		move_speed;
	double		rot_speed;
	t_keys		keys;
	t_texture	*gun_anim;
	int			current_frame;
	int			is_firing;
	double		last_step;
	double		last_fire;
	int			gun_width;
	int			gun_height;
	int			health;
}				t_player;

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	int			floor_color;
	int			ceiling_color;
	char		player_start;
	t_vector	player_pos;
}				t_map;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img[2];
	char			*addr[2];
	int				current_buffer;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				fd_map;
	char			*map_path;
	t_map			map;
	t_player		p1;
	t_texture		north;
	t_texture		south;
	t_texture		east;
	t_texture		west;
	int				has_skybox;
	char			*skybox_path;
	t_texture		skybox;
	int				has_floor;
	char			*floor_path;
	t_texture		floor;
	t_vector		last_mouse;
	double			mouse_sensi;
	double			fps;
	t_enemy_list	*enemies;
	t_door_system	*door_system;
	t_portal_system *portal_system;
}				t_game;

void			cleanup_game(t_game *game);
// parse
void			init_player(t_game *game);
bool			init_window(t_game *game);
void			init_game(t_game *game);

//events/
void			*keys_else(t_game *game, double *dir_x, double *dir_y);
//initialize/init_colors.c
void			*init_colors(t_game *game);
void			assign_color(t_game *game, char *line);
//init_colors2.c
int				get_number(int flag);
void			norm_norm(t_game *game, char *line, char **c, char **f);
int				rgb_to_hex(char *color);
//initialize/init_map.c
void			init_map(t_game *game);
char			**read_map(t_game *game);
int				count_lines(t_game *game);
void			set_grid(t_game *game);
//initialize/init_floor.c
int				is_floor_line(char *line);
void			process_floor_config(t_game *game, char *line);
void			init_floor(t_game *game);
//initialize/init_skybox.c
void			init_skybox(t_game *game);
void			process_skybox_config(t_game *game, char *line);
int				is_skybox_line(char *line);
//initialize/textures.c
void			init_textures(t_game *game);
//initialize/printers.c
void			print_map(t_game *game);
//
void			read_error(t_game *game);
//error/cleanup
void			cleanup_gun(t_game *game);
void			cleanup_textures(t_game *game);
void			cleanup_map(t_game *game);
void			cleanup_game(t_game *game);
void			cleanup_door_system(t_game *game);

//
void			init_test_map(t_game *game);
int				close_window(t_game *game);
void			handle_movement(t_game *game);
int				key_release(int keycode, t_game *game);
int				key_press(int keycode, t_game *game);
int				key_handler(int keycode, t_game *game);

#endif
