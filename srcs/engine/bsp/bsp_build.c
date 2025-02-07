/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Counts how lines are distributed relative to partition
** Updates front/back counters and tracks splits
** Updates total splits and depth metrics in count data
*/
void	count_line_sides(t_bsp_line *line, t_bsp_line *partition,
		t_count_data *count)
{
	t_bsp_side	side;

	side = bsp_classify_line(line, partition);
	if (side == BSP_FRONT)
		count->front++;
	else if (side == BSP_BACK)
		count->back++;
	else if (side == BSP_SPANNING)
	{
		count->split++;
		count->total_splits++;
	}
	if (count->tree_depth > count->max_depth)
		count->max_depth = count->tree_depth;
}

/*
** Evaluate partition quality using count data and depth
** Returns weighted score based on balance and splits
** Lower score indicates better partition
*/
t_fixed32	eval_partition(t_bsp_line *partition, t_bsp_line **lines,
		int num_lines, int depth)
{
	t_count_data	count;
	t_fixed32		balance_score;
	t_fixed32		split_score;
	t_fixed32		depth_penalty;

	init_count_data(&count);
	count.tree_depth = depth;
	while (num_lines--)
	{
		if (lines[num_lines] != partition)
			count_line_sides(lines[num_lines], partition, &count);
	}
	balance_score = int_to_fixed32(abs(count.front - count.back));
	split_score = int_to_fixed32(count.split * BSP_WEIGHT_FACTOR);
	depth_penalty = int_to_fixed32((depth * depth) / BSP_MAX_DEPTH);

	return (fixed32_add(fixed32_add(balance_score, split_score), depth_penalty));
}

/*
** Finds best partition line using multiple random seeds
** Tests different configurations for optimal balance
** Returns line that creates most balanced tree
*/
t_bsp_line	*choose_partition(t_bsp_line **lines, int num_lines)
{
	t_bsp_line	*best_line;
	t_fixed32	best_score;
	t_fixed32	curr_score;
	int			i;
	int			attempt;

	best_line = NULL;
	best_score = INT32_MAX;
	attempt = 0;
	while (attempt++ < BSP_MAX_SEED)
	{
		shuffle_lines(lines, num_lines, generate_random_seed());
		i = 0;
		while (i < num_lines)
		{
			curr_score = eval_partition(lines[i], lines, num_lines, 0);
			if (curr_score < best_score)
			{
				best_score = curr_score;
				best_line = lines[i];
			}
			i++;
		}
	}
	return (best_line);
}

/*
** Handles recursive build of BSP subtrees with depth tracking
** Creates front and back child nodes maintaining tree balance
** Returns pointer to parent node or NULL on failure
*/
t_bsp_node	*build_subtrees(t_bsp_node *node, t_bsp_data *data)
{
	if (!node || !data)
		return (NULL);
	node->front = build_bsp_tree(data->front_lines, data->num_front,
			node->depth + 1);
	node->back = build_bsp_tree(data->back_lines, data->num_back,
			node->depth + 1);
	free(data->front_lines);
	free(data->back_lines);
	return (node);
}

/*
** Builds BSP tree recursively with depth tracking
** Uses balanced partition selection at each level
** Returns NULL if max depth reached or build fails
*/
t_bsp_node	*build_bsp_tree(t_bsp_line **lines, int num_lines, int depth)
{
	t_bsp_node	*node;
	t_bsp_data	split_data;
	t_bsp_line	*partition;

	if (!lines || !num_lines || depth >= BSP_MAX_DEPTH)
		return (NULL);
	node = create_bsp_node();
	if (!node)
		return (NULL);
	node->depth = depth;
	partition = choose_partition(lines, num_lines);
	if (!partition)
		return (free_and_return(node, NULL));
	if (!split_lines(partition, lines, num_lines, &split_data))
		return (free_and_return(node, NULL));
	node->partition = partition;
	return (build_subtrees(node, &split_data));
}