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

void debug_print_lines(t_bsp_line **lines, int num_lines)
{
    int i;

    ft_printf("Lines to process (%d):\n", num_lines);
    for (i = 0; i < num_lines; i++)
    {
        ft_printf("  Line %d: (%d,%d) -> (%d,%d)\n", i,
            fixed32_to_int(lines[i]->start.x),
            fixed32_to_int(lines[i]->start.y),
            fixed32_to_int(lines[i]->end.x),
            fixed32_to_int(lines[i]->end.y));
    }
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
	if (data->num_front > 0)
		node->front = build_bsp_tree(data->front_lines, data->num_front,
				node->depth + 1);
	else
		node->front = NULL;
	if (data->num_back > 0)
		node->back = build_bsp_tree(data->back_lines, data->num_back,
				node->depth + 1);
	else
		node->back = NULL;
	free(data->front_lines);
	free(data->back_lines);
	return (node);
}

/*
** Builds BSP tree recursively with depth tracking
** Uses balanced partition selection at each level
** Returns NULL if max depth reached or build fails
*/
t_bsp_node *build_bsp_tree(t_bsp_line **lines, int num_lines, int depth)
{
    t_bsp_node *node;
    t_bsp_data split_data;

    if (!lines || num_lines <= 0)
        return (NULL);
    node = create_bsp_node();
    if (!node)
        return (NULL);
    node->depth = depth;
    node->partition = choose_partition(lines, num_lines);
    if (!node->partition)
    {
        node->lines = lines;
        node->num_lines = num_lines;
        return (node);
    }
    if (!split_lines(node->partition, lines, num_lines, &split_data))
    {
        free(node);
        return (NULL);
    }
    node->lines = NULL;
    node->num_lines = 0;
    return (build_subtrees(node, &split_data));
}