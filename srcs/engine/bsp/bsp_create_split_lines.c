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
static t_fixed_vec32  find_intersection(t_bsp_line *line, t_bsp_line *part)
{
    t_fixed_vec32  delta;
    t_fixed_vec32  part_delta;
    t_fixed32      den;
    t_fixed32      t;

    delta.x = fixed32_sub(line->end.x, line->start.x);
    delta.y = fixed32_sub(line->end.y, line->start.y);
    
    part_delta.x = fixed32_sub(part->end.x, part->start.x);
    part_delta.y = fixed32_sub(part->end.y, part->start.y);

    den = fixed32_sub(
        fixed32_mul(delta.x, part_delta.y),
        fixed32_mul(delta.y, part_delta.x)
    );

    t = fixed32_div(
        fixed32_sub(
            fixed32_mul(part_delta.x, fixed32_sub(line->start.y, part->start.y)),
            fixed32_mul(part_delta.y, fixed32_sub(line->start.x, part->start.x))
        ),
        den
    );

    return ((t_fixed_vec32){
        fixed32_add(line->start.x, fixed32_mul(delta.x, t)),
        fixed32_add(line->start.y, fixed32_mul(delta.y, t))
    });
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
		return (false);
	}
	return (true);
}

/*
** Classifies points and creates split lines
** Sets front and back pointers to NULL if split fails
*/
static void	classify_and_split(t_bsp_line *line, t_bsp_line *partition,
		t_bsp_line **front, t_bsp_line **back)
{
	t_fixed_vec32	intersect;
	t_bsp_side		start_side;
	//t_bsp_side		end_side;

	*front = NULL;
	*back = NULL;
	intersect = find_intersection(line, partition);
	start_side = bsp_classify_point(line->start, partition);
	//end_side = bsp_classify_point(line->end, partition);
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

/*
** Main function to split a BSP line at intersection with partition
** Returns false if split fails
*/
bool	split_bsp_line(t_bsp_line *line, t_bsp_line *partition,
		t_bsp_line **front, t_bsp_line **back)
{
	classify_and_split(line, partition, front, back);
	if (*front == NULL || *back == NULL)
		return (false);
	return (true);
}
