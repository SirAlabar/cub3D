#ifndef TYPES_H
# define TYPES_H

# include <stdint.h>
# include <stdbool.h>

/* Forward declarations of all major structs */
typedef struct s_game t_game;
typedef struct s_player t_player;
typedef struct s_vector t_vector;
typedef struct s_vector_i t_vector_i;
typedef struct s_texture t_texture;
typedef struct s_doom_map t_doom_map;
typedef struct s_sector t_sector;
typedef struct s_keys t_keys;
typedef struct s_cmd t_cmd;
typedef struct s_bsp_tree t_bsp_tree;
typedef struct s_bsp_line t_bsp_line;
typedef struct s_bsp_node t_bsp_node;
typedef int32_t		t_fixed32;
typedef int64_t		t_fixed64;

#endif