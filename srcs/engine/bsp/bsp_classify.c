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
    if (!partition)
        return BSP_COLINEAR;

    // Vetores
    t_fixed_vec32 vec_to_point = {
        fixed32_sub(point.x, partition->start.x),
        fixed32_sub(point.y, partition->start.y)
    };

    t_fixed_vec32 line_vec = {
        fixed32_sub(partition->end.x, partition->start.x),
        fixed32_sub(partition->end.y, partition->start.y)
    };

    // Cálculo do produto cruz (determinante)
    // Corrigido: Invertemos a ordem para corrigir a orientação
    t_fixed32 cross_product = fixed32_sub(
        fixed32_mul(line_vec.x, vec_to_point.y),
        fixed32_mul(line_vec.y, vec_to_point.x)
    );

    // Epsilon adaptativo baseado no comprimento da linha
    t_fixed32 line_length_sq = fixed32_add(
        fixed32_mul(line_vec.x, line_vec.x),
        fixed32_mul(line_vec.y, line_vec.y)
    );
    t_fixed32 epsilon = fixed32_mul(
        fixed32_sqrt(line_length_sq),
        int_to_fixed32(1) >> 6
    );

    // Classificação com epsilon adaptativo
    if (cross_product > epsilon)
        return BSP_FRONT;
    if (cross_product < -epsilon)
        return BSP_BACK;

    // Verificação para pontos além dos endpoints
    t_fixed32 dot_product = fixed32_add(
        fixed32_mul(vec_to_point.x, line_vec.x),
        fixed32_mul(vec_to_point.y, line_vec.y)
    );

    if (dot_product < 0 || dot_product > line_length_sq)
        return BSP_COLINEAR;

    return BSP_COLINEAR;
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
