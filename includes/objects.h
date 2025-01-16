/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by your_login        #+#    #+#             */
/*   Updated: 2025/01/15 10:00:00 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include <cub3d.h>
# include <stdbool.h>

typedef struct s_vector t_vector;
typedef struct s_vector_i t_vector_i;
typedef struct s_ray t_ray;
typedef struct s_texture t_texture;
typedef struct s_game t_game;

/* Configurações gerais */
# define MIN_MAP_SIZE 5
# define MIN_EMPTY_SPACES 10
# define MIN_SPACE_BETWEEN_OBJECTS 2.0
# define MIN_SPACE_FROM_WALL 0.5
# define MAX_SPACE_FROM_WALL 1.5
# define OBJECTS_PER_SPACE 9
# define MAX_FRAMES 5

# define TEXTURE_SHIFT 8
# define TEXTURE_SCALE (1 << TEXTURE_SHIFT)
# define TEXTURE_HALF_SCALE (TEXTURE_SCALE / 2)

/* Base paths */
# define ASSETS_PATH     "assets/sprites"

/* Object paths */
# define PILLAR_PATH     ASSETS_PATH"/pillar.xpm"
# define BARREL_PATH     ASSETS_PATH"/barrel.xpm"
# define ARMOR_PATH      ASSETS_PATH"/armor.xpm"
# define PLANT_PATH      ASSETS_PATH"/plant.xpm"
# define LAMP_PATH       ASSETS_PATH"/lamp.xpm"
# define CRATE_PATH      ASSETS_PATH"/crate.xpm"
# define STATUE_PATH     ASSETS_PATH"/statue.xpm"

/* Torch animation paths */
# define TORCH_PATH      ASSETS_PATH"/torch"
# define TORCH_FRAME1    ASSETS_PATH"/torch1.xpm"
# define TORCH_FRAME2    ASSETS_PATH"/torch2.xpm"
# define TORCH_FRAME3    ASSETS_PATH"/torch3.xpm"
# define TORCH_FRAME4    ASSETS_PATH"/torch4.xpm"

typedef enum e_placement_type {
    PLACEMENT_WALL,
    PLACEMENT_AWAY,
    PLACEMENT_ANY
} t_placement_type;

typedef enum e_object_type {
    OBJ_PILLAR,
    OBJ_TORCH,
    OBJ_BARREL,
    OBJ_ARMOR,
    OBJ_PLANT,
    OBJ_LAMP,
    OBJ_CRATE,
    OBJ_STATUE,
    OBJ_TYPES_COUNT
} t_object_type;

typedef struct s_object_config {
    char            *texture_path;
    bool            is_solid;
    double          collision_radius;
    bool            is_animated;
    int             num_frames;
    char            *frame_paths[MAX_FRAMES];
    t_placement_type placement;    
} t_object_config;

typedef struct s_sprite {
    t_vector        pos;
    t_vector_i      tex;
    double          distance;
    t_object_type   type;
    bool            is_solid;
    bool            is_animated;
    int             current_frame;
    double          anim_time;
    t_texture       *frames[4];
} t_sprite;

typedef struct s_object_system {
    t_sprite    *sprites;
    int         num_sprites;
    int         max_sprites;
} t_object_system;


void    transform_sprite(t_game *game, t_sprite *sprite, double *transform_x,
        double *transform_y);
void    set_sprite_limits(int *start, int *end, int sprite_size, int max);
void    calculate_dimensions(t_game *game, t_sprite *sprite, double transform_x,
        double transform_y);


void    init_object_config(t_object_config *config, t_object_type type);
bool    init_sprite(t_game *game, t_sprite *sprite, t_vector pos, 
        t_object_type type);
bool    try_place_object(t_game *game, t_object_type type);


bool    load_sprite_textures(t_game *game, t_sprite *sprite);
bool    load_sprite_animations(t_game *game, t_sprite *sprite,
        t_object_config *config);


bool    validate_map_for_objects(t_game *game, int *available_spaces);
bool validate_tex_obj(t_object_type type);
int     calculate_max_objects(int available_spaces);
bool    is_valid_object_position(t_game *game, t_vector pos, 
        t_placement_type placement);
bool    is_near_door(t_game *game, t_vector pos);
bool    check_object_spacing(t_game *game, t_vector pos);


t_vector get_random_position(t_game *game, t_placement_type placement);


void    render_sprites(t_game *game);

bool	try_place_object(t_game *game, t_object_type type);
void    update_objects_animations(t_game *game, double delta_time);


void    cleanup_sprites(t_game *game);
void    destroy_sprite(t_game *game, t_sprite *sprite);

#endif