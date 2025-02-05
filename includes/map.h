/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include <fixed_point.h>
//# include <bsp.h>

/*
 * Maximum limits for map elements
 * These values balance memory usage with map complexity
 * Can be adjusted based on game requirements
*/
# define MAX_VERTICES 1024
# define MAX_LINEDEFS 2048
# define MAX_SIDEDEFS 4096
# define MAX_SECTORS 256
# define MAX_THINGS 512
# define MAX_TEXTURE_NAME 32

# define ERROR_USAGE "Game usage: ./cub3d maps/valid_map.cub\n"
# define ERROR_MAPTYPE "Invalid map extension\n"
# define ERROR_OPEN "Error\nOpen"

typedef enum e_section
{
	NONE,
	VERTICES,
	SIDEDEFS,	
	LINEDEFS,
	SECTORS,
	THINGS
}	t_section;

/*
 * Vertex structure for map geometry
 * Uses fixed-point arithmetic for precise positioning
 * Each vertex can be shared by multiple linedefs
*/
typedef struct s_vertex
{
	t_fixed_vec32	pos;
}	t_vertex;

typedef struct s_sidedef
{
    t_fixed32    x_offset;
    t_fixed32    y_offset;
    char        *upper_texture;
    char        *middle_texture;
    char        *lower_texture;
    int         sector;
} t_sidedef;
/*
 * Linedef structure defining wall segments
 * Connects two vertices and defines properties of the wall
 * - vertex1/2: Indices to vertex array defining line endpoints
 * - front/back_sector: Sectors on each side (-1 for solid wall)
 * - type: Wall properties (0:solid, 1:door, 2:transparent)
 * - length: Precalculated length for efficiency
*/
typedef struct s_linedef
{
	int				vertex1;
	int				vertex2;
    int				front_sidedef;
    int				back_sidedef;
	int				type;
	t_fixed32		length;
}	t_linedef;

/*
 * Sector structure defining room properties
 * Each sector is a convex polygon with specific attributes
 * - floor/ceiling_height: Vertical space boundaries
 * - light: Sector brightness (0-255)
 * - textures: Names of floor/ceiling textures
 * - linedefs: Array of linedef indices forming sector boundary
*/
typedef struct s_sector
{
	t_fixed32		floor_height;
	t_fixed32		ceiling_height;
	int				light;
    char            *floor_texture;
    char            *ceiling_texture;
	int				linedef_count;
	int				*linedefs;
}	t_sector;

/*
 * Thing structure for map objects
 * Represents players, enemies, items, etc.
 * - type: Object type (1:player, 2:enemy, 3:item)
 * - pos: Fixed-point position in map
 * - angle: Orientation in BAM (Binary Angular Measurement)
*/
typedef struct s_thing
{
	int				type;
	t_fixed_vec32	pos;
	t_fixed32		angle;
}	t_thing;

/*
 * Main map structure containing all map data
 * Organized arrays of map elements with counters
 * Used for both map loading and runtime reference
*/
typedef struct s_doom_map
{
	t_vertex		vertices[MAX_VERTICES];
	int				vertex_count;
    t_sidedef       sidedefs[MAX_SIDEDEFS];
    int             sidedef_count;        
	t_linedef		linedefs[MAX_LINEDEFS];
	int				linedef_count;
	t_sector		sectors[MAX_SECTORS];
	int				sector_count;
	t_thing			things[MAX_THINGS];
	int				thing_count;
}	t_doom_map;

/*
** Function prototypes
*/

/* map_parse.c */
bool    parse_map(int fd, t_doom_map *map);
bool    load_map(int argc, char **argv, t_doom_map *map);

char	*clean_line(char *line);

/* map_vertises_parse.c */
bool	parse_vertices_section(char *line, t_doom_map *map);

/* map_sidedefs_parse.c */
bool get_sidedef_data(char *data, t_sidedef *sidedef);
bool    parse_sidedefs_section(char *line, t_doom_map *map);

/* map_sidedefs_utils.c */
bool    get_sidedef_number(char *str, int *number);
bool    parse_texture_path(char *str, char **texture_path);
bool    validate_sidedef_textures(t_sidedef *sidedef);

/* map_section_parse.c */
bool    parse_vertices_section(char *line, t_doom_map *map);
bool    parse_linedefs_section(char *line, t_doom_map *map);
bool    parse_sectors_section(char *line, t_doom_map *map);
bool    parse_things_section(char *line, t_doom_map *map);

/* map_utils.c */
void    init_map(t_doom_map *map);
void    cleanup_map(t_doom_map *map);
void    free_split(char **split);

/* map_section_utils.c */
bool    get_sector_number(char *str, int *number);
bool    validate_heights(t_sector *sector, int floor, int ceiling);
bool    validate_light(t_sector *sector, int light);

/* map_linedef_utils.c */
bool    get_linedef_number(char *str, int *number);

/* validate_map.c */
bool    validate_map_extension(int argc, char **argv);
bool    valid_extension(char *map);
int     open_map(int argc, char **argv);
bool	validate_map(t_doom_map *map);

/* map_validate_sidedefs.c */
bool    validate_sidedef(t_sidedef *sidedef, t_doom_map *map);
bool	validate_sidedef_references(t_doom_map *map);
bool    validate_all_sidedefs(t_doom_map *map);

/* map_validate_player.c */
bool    validate_player(t_doom_map *map);
bool    validate_things(t_doom_map *map);

/* texture_validate.c */
bool    texture_exists(char *texture_path);
bool    validate_all_textures(t_doom_map *map);



/* linedef_validate.c */
bool    check_linedef_intersect(t_linedef *line, t_sector *sector, t_doom_map *map);
bool    validate_linedef(t_linedef *line, t_doom_map *map);

/* map_validate_sectors.c */
bool    validate_sectors(t_doom_map *map);

#endif