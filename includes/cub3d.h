/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:34 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 18:38:37 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* Window settings */
# define WINDOW_WIDTH 1200
# define WINDOW_HEIGHT 900
# define FOV_BAM 715827882  /* 60 graus in BAM */
# define MOVE_SPEED 0.02
# define ROTATION_SPEED 0.04
# define MAX_ROTATION 0.045

# include <colors.h>
# include <types.h>
# include <fixed_point.h>
# include <bsp.h>
# include <engine.h>
# include <map.h>
# include <events.h>

# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <mlx_utils.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

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

#define PLAYER_HEIGHT 41
#define PLAYER_RADIUS 16
#define PLAYER_VIEW_HEIGHT 41
#define MAXMOVE 30
#define STOPSPEED 4
#define FRICTION 0xE800
#define GRAVITY 1
#define JUMPSPEED 8

/* Structs */

typedef struct s_player
{
    t_fixed_vec32    pos;
    t_fixed32       z;
    t_fixed32     view_z;
    t_fixed32     momx;
    t_fixed32     momy; 
    t_fixed32     momz;
    t_fixed32     angle;
    int         on_ground;
    t_cmd       cmd;
    t_keys      keys;
    int         health;
    int         armor;
    t_texture   *gun_anim;
    int         current_frame;
    int         is_firing;
    t_fixed32     last_fire;
    t_sector    *sector;
}               t_player;


typedef struct s_game
{
    void        *mlx;
    void        *win;
    void        *buffer[2];
    char        *addr[2];
    int         current_buffer;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
    t_doom_map  *map;
    t_player    p1;
    t_vector    last_mouse;
    double      mouse_sensi;
    double      fps;
    t_bsp_tree  *bsp_tree;
    t_fixed_tables_8192     *fixed_tables;	
}               t_game;

// parse
bool			init_player(t_game *game);
bool			init_game(t_game *game);
//events/
void			*keys_else(t_game *game, double *dir_x, double *dir_y);
//initialize/init_colors.c
void			*init_colors(t_game *game);
void			assign_color(t_game *game, char *line);
//init_colors2.c
int				get_number(int flag);
void			norm_norm(t_game *game, char *line, char **c, char **f);
int				rgb_to_hex(char *color);


//initialize/textures.c
bool			init_textures(t_game *game);
//initialize/printers.c
void			print_map(t_game *game);
//
void			read_error(t_game *game);
//error/cleanup

void	cleanup_bsp_tree(t_bsp_tree *tree);
void	cleanup_doom_map(t_doom_map *map);
void	cleanup_textures(t_game *game);
void	cleanup_game(t_game *game);

//
void			init_test_map(t_game *game);
int				close_window(t_game *game);
void			handle_movement(t_game *game);
int				key_release(int keycode, t_game *game);
int				key_press(int keycode, t_game *game);
int				key_handler(int keycode, t_game *game);

#endif
