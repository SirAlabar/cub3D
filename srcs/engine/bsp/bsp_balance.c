/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_balance.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Calculate balance score for a partition line
** Based on distribution of lines and potential splits
*/
static t_fixed32	calculate_balance_score(t_bsp_node *node, t_bsp_line *line)
{
	t_count_data	count;
	t_fixed32		balance_score;
	int				i;

	count.front = 0;
	count.back = 0;
	count.split = 0;
	i = 0;
	while (i < node->num_lines)
	{
		if (node->lines[i] != line)
			count_line_sides(node->lines[i], line, &count);
		i++;
	}
	balance_score = int_to_fixed32(abs(count.front - count.back));
	balance_score = fixed32_add(balance_score,
			int_to_fixed32(count.split * SPLIT_PENALTY));
	return (balance_score);
}

/*
** Find best partition line for balancing
** Returns line that provides best distribution
*/
static t_bsp_line	*find_best_partition(t_bsp_node *node)
{
	t_bsp_line	*best_line;
	t_fixed32	best_score;
	t_fixed32	curr_score;
	int			i;

	best_line = NULL;
	best_score = INT32_MAX;
	i = 0;
	while (i < node->num_lines)
	{
		curr_score = calculate_balance_score(node, node->lines[i]);
		if (curr_score < best_score)
		{
			best_score = curr_score;
			best_line = node->lines[i];
		}
		i++;
	}
	return (best_line);
}

/*
** Balance a single node in the tree
** Returns false if balancing fails
*/
static bool	balance_node(t_bsp_node *node)
{
	t_bsp_line	*new_partition;
	t_bsp_data	split_data;

	if (!node || node->num_lines <= 1)
		return (true);
	new_partition = find_best_partition(node);
	if (!new_partition)
		return (false);
	if (!split_lines(new_partition, node->lines, node->num_lines, &split_data))
		return (false);
	free(node->lines);
	node->partition = new_partition;
	node->lines = NULL;
	node->num_lines = 0;
	return (build_subtrees(node, &split_data) != NULL);
}

/*
** Balance entire BSP tree recursively
** Returns false if any node fails to balance
*/
bool	balance_bsp_tree(t_bsp_tree *tree)
{
	if (!tree || !tree->root)
		return (false);
	return (balance_node(tree->root));
}
