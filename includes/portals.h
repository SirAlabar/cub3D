/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/05 15:01:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PORTAL_H
# define PORTAL_H

# include <cub3d.h>

/* Portals*/
# define PORTAL_BLUE_TEX "./assets/texture/portal_blue.xpm"
# define PORTAL_ORANGE_TEX "./assets/texture/portal_orange.xpm"
# define PORTAL_DIST_THRESHOLD 1.0
# define PORTAL_DOT_THRESHOLD 0.5
# define MAX_PORTAL_DEPTH 3

// /* PORTAL Gun Frames*/
# define PORTAL_GUN "./assets/sprites/plasma/portal_gun.xpm"
# define PORTAL_GUN2 "./assets/sprites/plasma/portal_gun2.xpm"
# define PORTAL_GUN3 "./assets/sprites/plasma/portal_gun3.xpm"
# define PORTAL_GUN4 "./assets/sprites/plasma/portal_gun4.xpm"

typedef enum e_cardinal
{
	NORTH,
	SOUTH,
	EAST,
	WEST
}	t_cardinal;

typedef enum e_portal_type
{
	PORTAL_BLUE,
	PORTAL_ORANGE,
	PORTAL_INACTIVE
}	t_portal_type;

typedef enum e_portal_state
{
	PORTAL_CLOSED,
	PORTAL_OPENING,
	PORTAL_OPEN,
	PORTAL_CLOSING
}	t_portal_state;

typedef struct s_portal_gun
{
	t_texture	*blue_texture;
	t_texture	*orange_texture;
    t_texture   *gun_anim;
	double		last_fire_time;
	double		cooldown;
	int			active_portal;
    int         current_frame;
    int         is_firing;	
}	t_portal_gun;

typedef struct s_portal_wall
{
	t_vector_i	position;
	t_vector	normal;
	double		offset;
	t_portal_type	type;
	t_portal_state	state;
	double		animation;
	bool		active;
	double		timer;
	t_cardinal	card;
	struct s_portal_wall	*linked_portal;
}	t_portal_wall;

typedef struct s_portal_system
{
	t_portal_gun	gun;
	t_portal_wall	blue_portal;
	t_portal_wall	orange_portal;
	t_texture		portal_texture;
	bool			portal_active;
	double			last_teleport_time;
}	t_portal_system;

typedef struct s_portal_hit
{
	bool		found;
	t_vector_i	wall_pos;
	t_vector	hit_pos;
	t_vector	normal;
	double		offset;
	double		distance;
}	t_portal_hit;


/* portal_init.c */
void		init_portal_system(t_game *game);
void		cleanup_portal_system(t_game *game);

/* portal_detection.c */
t_portal_hit	detect_wall_for_portal(t_game *game);
bool		is_valid_portal_surface(t_game *game, t_portal_hit hit);

/* portal_creation.c */
void		create_portal(t_game *game, t_portal_hit hit, t_portal_type type);
void		fire_portal_gun(t_game *game);
void		update_portal_gun_animation(t_game *game);

/* portal_update.c */
void		update_portals(t_game *game);
void		handle_portal_gun_input(t_game *game, int keycode);
void		close_portals(t_game *game);

/* portal_teleport.c */
void		check_portal_teleport(t_game *game);

/* portal_render.c */
void		render_portal_wall(t_ray *ray, t_game *game, int x, t_scanline *buffer);
void		render_portals(t_game *game, t_ray *rays, t_scanline *buffer);
void		draw_portal_gun(t_game *game);

/* portal_raycasting.c */
bool process_portal_hit(t_ray *ray, t_game *game);
void translate_portal_ray(t_ray *ray, t_game *game, t_portal_wall *entry,
	t_portal_wall *exit);

/* portal_integration.c */
void		check_ray_portal_collision(t_ray *ray, t_game *game);
void		integrate_portals_with_raycasting(t_game *game, t_ray *rays);
void		update_ray_check_collisions(t_ray *ray, t_game *game);
void		integrate_portal_system(t_game *game);


bool	check_portal_hit(t_ray *ray, t_game *game, t_portal_wall *portal);
unsigned int get_portal_color(t_portal_wall *portal, double x, double y, t_game *game);
double	get_angle_between_normals(t_vector n1, t_vector n2);
t_vector	get_direction_vector(t_cardinal card);
t_cardinal	get_opposite_cardinal(t_cardinal card);

#endif