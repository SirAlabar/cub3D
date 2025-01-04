/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:39:31 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/04 19:03:05 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <engine.h>
# include <colors.h>
# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <mlx_utils.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <validations.h>

# ifdef MAC_OS
#  include <mlx.h>
#  define KEY_ESC 53
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_UP 126
#  define KEY_DOWN 125
# else
#  include <X11/X.h>
#  include <X11/keysym.h>
#  include <mlx.h>
#  define KEY_ESC XK_Escape
#  define KEY_W XK_w
#  define KEY_A XK_a
#  define KEY_S XK_s
#  define KEY_D XK_d
#  define KEY_LEFT XK_Left
#  define KEY_RIGHT XK_Right
#  define KEY_UP XK_Up
#  define KEY_DOWN XK_Down
# endif

/* Window settings */
# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
# define FOV 60
# define MOVE_SPEED 0.1
# define ROTATION_SPEED 0.05

/* Map characters */
# define VALID_MAP_CHARS "01NSEW "
# define PLAYER_CHARS "NSEW"

/* Error messages */
# define ERR_ARGS "Error\nInvalid number of arguments\n"
# define ERR_FILE "Error\nInvalid file extension or unable to open file\n"
# define ERR_MAP "Error\nInvalid map configuration\n"
# define ERR_TEXTURE "Error\nInvalid texture configuration\n"
# define ERR_COLOR "Error\nInvalid color configuration\n"
# define ERR_MALLOC "Error\nMemory allocation failed\n"

/* Structs */

typedef struct s_player
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
	double		move_speed;
	double		rot_speed;
}				t_player;

typedef struct s_texture
{
	void		*img;
	char		*addr;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
	char		*path;
}				t_texture;

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
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	t_map		map;
	t_player	p1;
	t_texture	north;
	t_texture	south;
	t_texture	east;
	t_texture	west;
}				t_game;

// put_pixel.c
void			put_pixel(t_texture *tex, int x, int y, int color);
unsigned int	get_color_in_pixel(t_texture *tex, int x, int y);

// texture.c
t_texture		*create_texture(t_game *game, char *texture_path);
void			destroy_texture(t_texture **texture, void *mlx);

// parse
void			init_player(t_game *game);
void			init_game(t_game *game);

// render.c
void			draw_floor_ceiling(t_game *game);
void			render_frame(t_game *game);

#endif
