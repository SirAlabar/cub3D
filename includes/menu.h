/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:49:34 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/09 16:43:40 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

typedef struct s_texture    t_texture;

# define MAX_FRAMES 10

# define BG1 "./assets/texture/bg1.xpm"
# define BG2 "./assets/texture/bg2.xpm"
# define BG3 "./assets/texture/bg3.xpm"
# define BG4 "./assets/texture/bg4.xpm"

# define CUBE1 "./assets/texture/cube1.xpm"
# define CUBE2 "./assets/texture/cube2.xpm"
# define CUBE3 "./assets/texture/cube3.xpm"
# define CUBE4 "./assets/texture/cube4.xpm"
# define CUBE5 "./assets/texture/cube5.xpm"

# define START1 "./assets/texture/start1.xpm"
# define START2 "./assets/texture/start1.xpm"
# define START3 "./assets/texture/start1.xpm"
# define START4 "./assets/texture/start1.xpm"
# define START5 "./assets/texture/start1.xpm"
# define START6 "./assets/texture/start1.xpm"

# define EXIT1 "./assets/texture/exit1.xpm"
# define EXIT2 "./assets/texture/exit2.xpm"
# define EXIT3 "./assets/texture/exit3.xpm"
# define EXIT4 "./assets/texture/exit4.xpm"
# define EXIT5 "./assets/texture/exit5.xpm"
# define EXIT6 "./assets/texture/exit6.xpm"

# define SELECT_GUN "./assets/texture/select_gun.xpm"


typedef struct s_animation
{
	t_texture	frames[MAX_FRAMES];
	int			frame_count;
	int			current_frame;
	double		frame_duration;
	double		last_update;  
	int			active;
}				t_animation;

typedef struct s_menu_item
{
	t_animation	anim;
	int			selected;
}				t_menu_item;

typedef struct s_menu
{
	int			active;
	t_animation	background;
	t_animation	cube;
	t_menu_item	start_btn;
	t_menu_item	exit_btn;
	t_animation	select_gun;
	int			selected_option;
}				t_menu;


void        setup_menu_hooks(t_game *game);
int         init_menu(t_game *game);
int         menu_render_frame(t_game *game);

int         init_menu_background(t_game *game);
int         init_menu_cube(t_game *game);
int         init_menu_start(t_game *game);
int         init_menu_start(t_game *game);
int         init_select_gun(t_game *game);



#endif