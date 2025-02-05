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
*/
static bool	alloc_bsp_lines(t_bsp_line ***lines, int count)
{
	*lines = ft_calloc(count, sizeof(t_bsp_line *));
	if (!*lines)
		return (false);
	return (true);
}

/*
** Create BSP line from map linedef data
** Uses vertices positions and linedef type
** Returns NULL if creation fails
*/
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

/*
** Extract BSP lines from map geometry
** Creates array of BSP lines from map linedefs
** Returns false if extraction fails
*/
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
        bsp_lines[i] = create_line_from_linedef(map, &map->linedefs[i]);
        if (!bsp_lines[i])
        {
            while (--i >= 0)
                free(bsp_lines[i]);
            free(bsp_lines);
            return (false);
        }
    }

    *lines = bsp_lines;
    return (true);
}