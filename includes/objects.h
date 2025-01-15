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
# include <engine.h>
# include <stdbool.h>

/* Configurações gerais */
# define MAX_OBJECT_TEXTURES 8
# define MAX_FRAMES 4
# define ANIMATION_SPEED 0.15

/* Base paths */
# define ASSETS_PATH     "assets/textures/sprites"

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
} t_object_config;

typedef struct s_sprite {
    t_vector        pos;
    t_vector_i      tex;
    double          distance;
    int             texture_id;
    bool            is_solid;
    int             width;
    int             height;
    bool            is_animated;
    int             current_frame;
    int             num_frames;
    double          anim_time;
    double          frame_duration;
    t_texture       *frames[MAX_FRAMES];
} t_sprite;

/* Funções de gerenciamento de objetos */
void            init_object_system(t_game *game);
t_sprite        *create_sprite(t_game *game, t_object_type type, t_vector pos);
void            update_sprite_animations(t_game *game, double delta_time);
void            render_sprites(t_game *game);
void            cleanup_sprites(t_game *game);
bool            check_sprite_collision(t_game *game, t_vector pos);
t_texture       *get_sprite_texture(t_sprite *sprite);
void            destroy_sprite(t_game *game, t_sprite *sprite);

/* Funções de validação */
bool            verify_sprite_textures(t_object_type type);
bool            can_place_sprite(t_game *game, t_vector pos, double radius);
bool            is_valid_object_type(t_object_type type);

#endif