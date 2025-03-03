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
    t_fixed32 cross_product;
    t_fixed32 epsilon;
    t_fixed32 dot_product;

    if (!partition)
        return (BSP_COLINEAR);

    // Calcula o vetor do início da partição até o ponto
    vec_to_point = vector_sub_fixed32(point, partition->start);
    
    // Calcula o vetor da linha de partição
    line_vec = vector_sub_fixed32(partition->end, partition->start);
    
    // Calcula o quadrado do comprimento da linha
    line_length_sq = fixed32_add(
        fixed32_mul(line_vec.x, line_vec.x),
        fixed32_mul(line_vec.y, line_vec.y));

    // Define uma margem de erro mais generosa proporcional ao tamanho da linha
    epsilon = fixed32_mul(
        fixed32_sqrt(line_length_sq),
        int_to_fixed32(1) >> 4);  // Aumentada tolerância

    // Calcula o produto vetorial
    cross_product = fixed32_sub(
        fixed32_mul(line_vec.x, vec_to_point.y),
        fixed32_mul(line_vec.y, vec_to_point.x));

    // Se o ponto está muito próximo da linha, considera colinear
    if (fixed32_abs(cross_product) < epsilon)
    {
        // Verifica se o ponto está dentro do segmento de linha
        dot_product = fixed32_add(
            fixed32_mul(vec_to_point.x, line_vec.x),
            fixed32_mul(vec_to_point.y, line_vec.y));
            
        if (dot_product >= 0 && dot_product <= line_length_sq)
            return BSP_COLINEAR;
    }

    return (cross_product > 0 ? BSP_FRONT : BSP_BACK);
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

t_fixed32 eval_partition_quality(t_bsp_line *partition, t_bsp_line **lines, 
                                int num_lines, int depth)
{
    t_count_data count;
    t_fixed32 balance_score;
    t_fixed32 split_score;
    t_fixed32 depth_penalty;
    int i;

    init_count_data(&count);
    count.tree_depth = depth;
    
    i = -1;
    while (++i < num_lines)
    {
        if (lines[i] != partition)
            count_line_sides(lines[i], partition, &count);
    }
    
    // Calcular pontuação baseado no desbalanceamento e penalidade por divisões
    balance_score = int_to_fixed32(abs(count.front - count.back));
    split_score = int_to_fixed32(count.split * SPLIT_PENALTY);
    depth_penalty = int_to_fixed32((depth * depth) / BSP_MAX_DEPTH);

    return fixed32_add(fixed32_add(balance_score, split_score), depth_penalty);
}
/*
** Finds best partition line using multiple random seeds
** Tests different configurations for optimal balance
** Returns line that creates most balanced tree
*/
t_bsp_line *choose_partition(t_bsp_line **lines, int num_lines, t_thread_pool *pool)
{
	t_bsp_line	*best_line;
	t_fixed32	best_score;
	t_fixed32	curr_score;
	int			i;
	unsigned int seed;

	if (!lines || num_lines <= 0)
		return (NULL);
	seed = find_best_seed(lines, num_lines, 0, pool);
	shuffle_lines(lines, num_lines, seed);
	best_line = lines[0];
	best_score = eval_partition_quality(best_line, lines, num_lines, 0);
	i = -1;
	while (++i < num_lines)
	{
		curr_score = eval_partition_quality(lines[i], lines, num_lines, 0);
		if (curr_score < best_score)
		{
			best_score = curr_score;
			best_line = lines[i];
		}
	}
	ft_printf("Selected partition with score %d\n", fixed32_to_int(best_score));
	return (best_line);
}

// t_fixed32	eval_seed(unsigned int seed, t_bsp_line **lines,
// 	int num_lines)
// {
// 	t_bsp_line	**shuffled;
// 	t_bsp_line	*partition;
// 	t_fixed32	score;
// 	int			i;

// 	shuffled = (t_bsp_line **)malloc(sizeof(t_bsp_line *) * num_lines);
// 	if (!shuffled)
// 	return (INT32_MAX);
// 	i = -1;
// 	while (++i < num_lines)
// 		shuffled[i] = lines[i];
// 	shuffle_lines(shuffled, num_lines, seed);
// 	partition = shuffled[0];
// 	score = eval_partition(partition, shuffled, num_lines, 0);
// 	free(shuffled);
// 	return (score);
// }