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
		count->split++;
}

/*
** Evaluates the quality of a potential partition line
** Calculates a score based on balance and splits
** Higher score for balanced tree, lower score for more split lines
** Returns a score value, higher is better
*/
t_fixed32	eval_partition(t_bsp_line *partition, t_bsp_line **lines,
		int num_lines)
{
	t_count_data	count;
	int			i;

	count.front = 0;
	count.back = 0;
	count.split = 0;
	i = 0;
	while (i < num_lines)
	{
		if (lines[i] != partition)
			count_line_sides(lines[i], partition, &count);
		i++;
	}
	return (int_to_fixed32(abs(count.front - count.back))
		- int_to_fixed32(count.split * 8));
}

/*
** Chooses the best partition line from a set of lines
** Uses evaluate_partition to score each potential partition
** Returns the line that creates the most balanced tree with fewest splits
*/
t_bsp_line	*choose_partition(t_bsp_line **lines, int num_lines)
{
	t_bsp_line	*best_partition;
	t_fixed32	best_score;
	t_fixed32	score;
	int		i;

	best_partition = NULL;
	best_score = INT32_MIN;
	i = 0;
	while (i < num_lines)
	{
		score = eval_partition(lines[i], lines, num_lines);
		if (score > best_score)
		{
			best_score = score;
			best_partition = lines[i];
		}
		i++;
	}
	return (best_partition);
}

/*
** Handles recursive build of BSP subtrees
*/
t_bsp_node	*build_subtrees(t_bsp_node *node, t_bsp_data *data)
{
	node->front = build_bsp_tree(data->front_lines, data->num_front);
	node->back = build_bsp_tree(data->back_lines, data->num_back);
	free(data->front_lines);
	free(data->back_lines);
	return (node);
}

/*
** Main recursive function to build the BSP tree
*/
t_bsp_node	*build_bsp_tree(t_bsp_line **lines, int num_lines)
{
	t_bsp_node	*node;
	t_bsp_data	data;

	if (num_lines == 0)
		return (NULL);
	node = create_bsp_node();
	if (!node)
		return (NULL);
	node->partition = choose_partition(lines, num_lines);
	if (!node->partition)
		return (free_and_return(node, NULL));
	if (!split_lines(node->partition, lines, num_lines, &data))
		return (free_and_return(node, NULL));
	return (build_subtrees(node, &data));
}
