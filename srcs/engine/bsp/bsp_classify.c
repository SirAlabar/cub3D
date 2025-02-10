/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_classify.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 10:00:00 by assistant         #+#    #+#             */
/*   Updated: 2025/02/01 10:00:00 by assistant        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

t_bsp_side bsp_get_side(t_fixed_vec32 vec_to_point, t_fixed_vec32 line_vec,
	t_fixed32 line_length_sq)
{
	t_fixed32 cross_product;
	t_fixed32 epsilon;
	t_fixed32 dot_product;

	cross_product = fixed32_sub(
		fixed32_mul(line_vec.x, vec_to_point.y),
		fixed32_mul(line_vec.y, vec_to_point.x)
	);
	epsilon = fixed32_mul(
		fixed32_sqrt(line_length_sq),
		int_to_fixed32(1) >> 6
	);
	if (cross_product > epsilon)
		return (BSP_FRONT);
	if (cross_product < -epsilon)
		return (BSP_BACK);
	dot_product = fixed32_add(
		fixed32_mul(vec_to_point.x, line_vec.x),
		fixed32_mul(vec_to_point.y, line_vec.y)
	);
	if (dot_product < 0 || dot_product > line_length_sq)
		return (BSP_COLINEAR);
	return (BSP_COLINEAR);
}

/*
** Determines which side of a partition line a point lies on using fixed-point
	math.
** Returns:
** - BSP_FRONT if point is in front of the line
** - BSP_BACK if point is behind the line
** - BSP_ON if point lies on the line
** Uses cross product in fixed-point for efficient classification
*/
t_bsp_side bsp_classify_point(t_fixed_vec32 point, t_bsp_line *partition)
{
	t_fixed_vec32 vec_to_point;
	t_fixed_vec32 line_vec;
	t_fixed32 line_length_sq;

	if (!partition)
		return (BSP_COLINEAR);
	vec_to_point = vector_sub_fixed32(point, partition->start);
	line_vec = vector_sub_fixed32(partition->end, partition->start);
	line_length_sq = fixed32_add(
		fixed32_mul(line_vec.x, line_vec.x),
		fixed32_mul(line_vec.y, line_vec.y)
	);
	return (bsp_get_side(vec_to_point, line_vec, line_length_sq));
}

/*
** Classifies a line segment relative to a partition line
** Returns:
** - BSP_FRONT if line is entirely in front
** - BSP_BACK if line is entirely behind
** - BSP_SPANNING if line crosses the partition
** - BSP_ON if line lies on the partition
*/
t_bsp_side	bsp_classify_line(t_bsp_line *line, t_bsp_line *partition)
{
	t_bsp_side	start_side;
	t_bsp_side	end_side;

	start_side = bsp_classify_point(line->start, partition);
	end_side = bsp_classify_point(line->end, partition);
	if (start_side == end_side)
		return (start_side);
	if (start_side == BSP_COLINEAR)
		return (end_side);
	if (end_side == BSP_COLINEAR)
		return (start_side);
	return (BSP_SPANNING);
}
