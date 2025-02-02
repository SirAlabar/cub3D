/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_linedef_validate.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:45:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 23:45:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Checks if two lines intersect using parametric equations
 * Returns true if lines intersect at a point other than their endpoints
 */
static bool	check_line_intersection(t_linedef *line1, t_linedef *line2,
		t_doom_map *map)
{
	t_fixed_vec32	p1;
	t_fixed_vec32	p2;
	t_fixed_vec32	p3;
	t_fixed_vec32	p4;
	t_fixed32		denom;

	p1 = map->vertices[line1->vertex1].pos;
	p2 = map->vertices[line1->vertex2].pos;
	p3 = map->vertices[line2->vertex1].pos;
	p4 = map->vertices[line2->vertex2].pos;
	denom = fixed32_sub(fixed32_mul(fixed32_sub(p4.y, p3.y),
				fixed32_sub(p2.x, p1.x)), fixed32_mul(fixed32_sub(p4.x, p3.x),
				fixed32_sub(p2.y, p1.y)));
	if (denom == 0)
		return (false);
	return (true);
}

/*
 * Checks if a linedef intersects with any linedef in a sector
 * Excludes connected linedefs (sharing vertices)
 * Returns true if any non-connected intersection is found
 */
bool	check_linedef_intersect(t_linedef *line, t_sector *sector,
		t_doom_map *map)
{
	int			i;
	t_linedef	*other;

	i = 0;
	while (i < sector->linedef_count)
	{
		other = &map->linedefs[sector->linedefs[i]];
		if (line->vertex1 != other->vertex1 && line->vertex1 != other->vertex2
			&& line->vertex2 != other->vertex1 && line->vertex2 != other->vertex2)
		{
			if (check_line_intersection(line, other, map))
			{
				ft_putendl_fd("Error\nIntersecting linedefs detected", 2);
				return (true);
			}
		}
		i++;
	}
	return (false);
}

/*
 * Validates basic linedef properties
 * Checks if vertices exist and are different
 * Returns false if linedef is invalid
 */
bool	validate_linedef(t_linedef *line, t_doom_map *map)
{
	if (line->vertex1 >= map->vertex_count || line->vertex2 >= map->vertex_count)
	{
		ft_putendl_fd("Error\nLinedef references nonexistent vertex", 2);
		return (false);
	}
	if (line->vertex1 == line->vertex2)
	{
		ft_putendl_fd("Error\nLinedef start and end vertices are the same", 2);
		return (false);
	}
	if (line->type < 0 || line->type > 2)
	{
		ft_putendl_fd("Error\nInvalid linedef type", 2);
		return (false);
	}
	return (true);
}
