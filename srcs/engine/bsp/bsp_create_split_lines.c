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
static t_fixed_vec32	find_intersection(t_bsp_line *line,
		t_bsp_line *partition)
{
	t_fixed_vec32	delta;
	t_fixed_vec32	delta_part;
	t_fixed32		denom;
	t_fixed32		t;

	delta = vector_sub(line->end, line->start);
	delta_part = vector_sub(partition->end, partition->start);
	denom = fixed32_mul(delta.x, delta_part.y) - fixed32_mul(delta.y,
			delta_part.x);
	t = fixed32_div(fixed32_mul(delta_part.x, fixed32_sub(line->start.y,
					partition->start.y)) - fixed32_mul(delta_part.y,
				fixed32_sub(line->start.x, partition->start.x)), denom);
	return ((t_fixed_vec32){fixed32_add(line->start.x, fixed32_mul(delta.x, t)),
		fixed32_add(line->start.y, fixed32_mul(delta.y, t))});
}

/*
** Creates a new line segment with given parameters
** Returns NULL if allocation fails
*/
static t_bsp_line	*create_split_line(t_fixed_vec32 start, t_fixed_vec32 end,
		int type)
{
	t_bsp_line	*line;

	line = malloc(sizeof(t_bsp_line));
	if (!line)
		return (NULL);
	line->start = start;
	line->end = end;
	line->type = type;
	line->normal = vector_normalize((t_fixed_vec32){fixed32_sub(end.y, start.y),
			fixed32_sub(start.x, end.x)});
	return (line);
}

/*
** Splits a line at intersection with partition
** Creates two new line segments if needed
*/
static bool	allocate_split_lines(t_bsp_line **front, t_bsp_line **back,
		t_bsp_line *line, t_fixed_vec32 intersect)
{
	*front = create_split_line(line->start, intersect, line->type);
	*back = create_split_line(intersect, line->end, line->type);
	if (!*front || !*back)
	{
		free(*front);
		free(*back);
		return (false);
	}
	return (true);
}

static void	classify_and_split(t_bsp_line *line, t_bsp_line *partition,
		t_bsp_line **front, t_bsp_line **back)
{
	t_fixed_vec32	intersect;
	t_bsp_side		start_side;
	t_bsp_side		end_side;

	*front = NULL;
	*back = NULL;
	intersect = find_intersection(line, partition);
	start_side = bsp_classify_point(line->start, partition);
	end_side = bsp_classify_point(line->end, partition);
	if (start_side == BSP_FRONT || start_side == BSP_COLINEAR)
	{
		if (!allocate_split_lines(front, back, line, intersect))
			return ;
	}
	else
	{
		if (!allocate_split_lines(back, front, line, intersect))
			return ;
	}
}

bool	split_bsp_line(t_bsp_line *line, t_bsp_line *partition,
		t_bsp_line **front, t_bsp_line **back)
{
	classify_and_split(line, partition, front, back);
	if (*front == NULL || *back == NULL)
		return (false);
	return (true);
}
