/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_map_lines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Alloc array for BSP lines based on map linedef count
** Returns false if allocation fails
*
static bool	alloc_bsp_lines(t_bsp_line ***lines, int count)
{
	*lines = ft_calloc(count, sizeof(t_bsp_line *));
	if (!*lines)
		return (false);
	return (true);
}*/

/*
** Create BSP line from map linedef data
** Uses vertices positions and linedef type
** Returns NULL if creation fails
*
static t_bsp_line *create_line_from_linedef(t_doom_map *map, t_linedef *linedef)
{
    t_fixed_vec32 start;
    t_fixed_vec32 end;

    if (linedef->vertex1 >= map->vertex_count || 
        linedef->vertex2 >= map->vertex_count)
        return (NULL);

    start = map->vertices[linedef->vertex1].pos;
    end = map->vertices[linedef->vertex2].pos;
    return (create_bsp_line(start, end, linedef->type));
}

*
** Extract BSP lines from map geometry
** Creates array of BSP lines from map linedefs
** Returns false if extraction fails
*
bool extract_map_lines(t_doom_map *map, t_bsp_line ***lines, int *num_lines)
{
    t_bsp_line **bsp_lines;
    int i;

    *num_lines = map->linedef_count;
    bsp_lines = ft_calloc(*num_lines, sizeof(t_bsp_line *));
    if (!bsp_lines)
        return (false);
    i = -1;
    while (++i < *num_lines)
    {
        t_linedef *current = &map->linedefs[i];
        bsp_lines[i] = create_bsp_line(map->vertices[current->vertex1].pos,
                                      map->vertices[current->vertex2].pos, 
                                      current->type);
        if (!bsp_lines[i])
        {
            while (--i >= 0)
                free(bsp_lines[i]);
            free(bsp_lines);
            return (false);
        }
        bsp_lines[i]->linedef_index = i;
        set_sector_info(map, current, bsp_lines[i]);
    }
    *lines = bsp_lines;
    return (true);
}
*/
static void set_sector_info(t_doom_map *map, t_linedef *linedef, t_bsp_line *line)
{
    if (linedef->front_sidedef >= 0 && 
        linedef->front_sidedef < map->sidedef_count)
        line->sector_id = map->sidedefs[linedef->front_sidedef].sector;
    else
        line->sector_id = -1;
    
    if (linedef->back_sidedef >= 0 && 
        linedef->back_sidedef < map->sidedef_count)
        line->neighbor_sector_id = map->sidedefs[linedef->back_sidedef].sector;
    else
        line->neighbor_sector_id = -1;
}

bool extract_map_lines(t_doom_map *map, t_bsp_line ***lines, int *num_lines)
{
    t_bsp_line **bsp_lines;
    int i;

    ft_printf("\n=== Detailed Map Line Extraction ===\n");
    ft_printf("Total vertices: %d\n", map->vertex_count);
    ft_printf("Total linedefs: %d\n", map->linedef_count);

    *num_lines = map->linedef_count;
    bsp_lines = ft_calloc(*num_lines, sizeof(t_bsp_line *));
    if (!bsp_lines)
    {
        ft_printf("Failed to allocate memory for BSP lines\n");
        return (false);
    }

    i = -1;
    while (++i < *num_lines)
    {
        // Detailed linedef info
        t_linedef *current_linedef = &map->linedefs[i];
        ft_printf("\nProcessing Linedef %d:\n", i);
        ft_printf("  Vertex1 index: %d\n", current_linedef->vertex1);
        ft_printf("  Vertex2 index: %d\n", current_linedef->vertex2);

        // Vertex validation
        if (current_linedef->vertex1 >= map->vertex_count || 
            current_linedef->vertex2 >= map->vertex_count)
        {
            ft_printf("  ERROR: Vertex index out of bounds\n");
            continue;
        }

        // Vertex coordinates
        t_fixed_vec32 start = map->vertices[current_linedef->vertex1].pos;
        t_fixed_vec32 end = map->vertices[current_linedef->vertex2].pos;

        ft_printf("  Start: (%d, %d)\n", 
            fixed32_to_int(start.x), 
            fixed32_to_int(start.y));
        ft_printf("  End: (%d, %d)\n", 
            fixed32_to_int(end.x), 
            fixed32_to_int(end.y));

        // Line creation
        bsp_lines[i] = create_bsp_line(start, end, current_linedef->type);
        
        if (!bsp_lines[i])
        {
            ft_printf("  Failed to create BSP line\n");
            // Clean up previously created lines
            while (--i >= 0)
                free(bsp_lines[i]);
            free(bsp_lines);
            return (false);
        }
        bsp_lines[i]->linedef_index = i;
        set_sector_info(map, current_linedef, bsp_lines[i]);
    }

    *lines = bsp_lines;
    return (true);
}