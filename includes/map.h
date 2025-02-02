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
# include <bsp.h>

/*
 * Maximum limits for map elements
 * These values balance memory usage with map complexity
 * Can be adjusted based on game requirements
*/
# define MAX_VERTICES 1024
# define MAX_LINEDEFS 2048
# define MAX_SECTORS 256
# define MAX_THINGS 512
# define MAX_TEXTURE_NAME 32

typedef enum e_section
{
	NONE,
	VERTICES,
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
	int				front_sector;
	int				back_sector;
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
	char			floor_texture[MAX_TEXTURE_NAME];
	char			ceiling_texture[MAX_TEXTURE_NAME];
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

/* doom_map_parse.c */
bool		parse_doom_map(const char *filename, t_doom_map *map);

/* doom_map_validate.c */
bool		validate_doom_map(t_doom_map *map);

/* doom_map_utils.c */
void		init_doom_map(t_doom_map *map);
void		cleanup_doom_map(t_doom_map *map);
t_fixed32	calculate_linedef_length(t_vertex v1, t_vertex v2);

/* doom_map_section_parse.c */
bool		parse_vertices_section(char *line, t_doom_map *map);
bool		parse_linedefs_section(char *line, t_doom_map *map);
bool		parse_sectors_section(char *line, t_doom_map *map);
bool		parse_things_section(char *line, t_doom_map *map);

#endif