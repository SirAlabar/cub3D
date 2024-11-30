/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:39:31 by marsoare          #+#    #+#             */
/*   Updated: 2024/11/30 21:33:18 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <X11/keysym.h>
# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/* Window settings */
# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720
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
typedef struct s_point
{
	double		x;
	double		y;
}				t_point;

typedef struct s_player
{
	t_point		pos;
	t_point		dir;
	t_point		plane;
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
	t_point		player_pos;
}				t_map;

typedef struct s_game
{
	void *mlx;  // point to mlx
	void *win;  // point to windown
	void *img;  // to main img
	char *addr; // point to data of main img
	t_map		map;
	t_player	player;
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

#endif
