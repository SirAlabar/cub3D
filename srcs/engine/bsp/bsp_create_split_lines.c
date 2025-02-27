/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_split_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>


/*
** Calculates intersection point between line and partition
** Returns intersection point in fixed point coordinates
*/
static t_fixed_vec32	find_intersection(t_bsp_line *line, t_bsp_line *part)
{
	t_fixed_vec32	delta;
	t_fixed_vec32	part_delta;
	t_fixed32		den;
	t_fixed32		t;

	delta.x = fixed32_sub(line->end.x, line->start.x);
	delta.y = fixed32_sub(line->end.y, line->start.y);
	part_delta.x = fixed32_sub(part->end.x, part->start.x);
	part_delta.y = fixed32_sub(part->end.y, part->start.y);
	den = fixed32_sub(fixed32_mul(delta.x, part_delta.y),
			fixed32_mul(delta.y, part_delta.x));
	if (den == 0)
		return ((t_fixed_vec32){0, 0});
	if (delta.x == 0)
		return ((t_fixed_vec32){line->start.x, part->start.y});
	t = fixed32_div(fixed32_sub(fixed32_mul(part_delta.x,
					fixed32_sub(line->start.y, part->start.y)),
				fixed32_mul(part_delta.y,
					fixed32_sub(line->start.x, part->start.x))), den);
	return ((t_fixed_vec32){fixed32_add(line->start.x, fixed32_mul(delta.x, t)),
		fixed32_add(line->start.y, fixed32_mul(delta.y, t))});
}


static bool	check_bounds(t_fixed_vec32 point, t_bsp_line *line)
{
	if (line->start.x == line->end.x)
		return (point.x == line->start.x
			&& point.y >= fix_min(line->start.y, line->end.y)
			&& point.y <= fix_max(line->start.y, line->end.y));
	return (point.x >= fix_min(line->start.x, line->end.x)
		&& point.x <= fix_max(line->start.x, line->end.x)
		&& point.y >= fix_min(line->start.y, line->end.y)
		&& point.y <= fix_max(line->start.y, line->end.y));
}

/*
** Splits a line at intersection with partition
** Creates two new line segments if needed
** Returns false if allocation fails
*/
static bool	allocate_split_lines(t_bsp_line **front, t_bsp_line **back,
		t_bsp_line *line, t_fixed_vec32 intersect)
{
	*front = create_bsp_line(line->start, intersect, line->type);
	*back = create_bsp_line(intersect, line->end, line->type);
	if (!*front || !*back)
	{
		free(*front);
		free(*back);
		*front = NULL;
		*back = NULL;
		return (false);
	}
    (*front)->linedef_index = line->linedef_index;
    (*front)->sector_id = line->sector_id;
    (*front)->neighbor_sector_id = line->neighbor_sector_id;
    (*back)->linedef_index = line->linedef_index;
    (*back)->sector_id = line->sector_id;
    (*back)->neighbor_sector_id = line->neighbor_sector_id;
	return (true);
}

/*
** Main function to split a BSP line at intersection with partition
** Returns false if split fails
*/
bool	split_bsp_line(t_bsp_line *line, t_bsp_line *partition,
		t_bsp_line **front, t_bsp_line **back)
{
	t_fixed_vec32	intersect;
	t_bsp_side		side;

	*front = NULL;
	*back = NULL;
	side = bsp_classify_line(line, partition);
	if (side != BSP_SPANNING)
		return (false);
	intersect = find_intersection(line, partition);
	if (!check_bounds(intersect, line) || !check_bounds(intersect, partition))
		return (false);
	if (bsp_classify_point(line->start, partition) == BSP_FRONT)
		return (allocate_split_lines(front, back, line, intersect));
	return (allocate_split_lines(back, front, line, intersect));
}