/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>


/*
* Free all lines stored in a node
* Safe to call with NULL pointers
*/
static void	free_node_lines(t_bsp_line **lines, int num_lines)
{
	int	i;

	if (!lines)
		return ;
	i = -1;
	while (++i < num_lines)
	{
		if (lines[i])
			free(lines[i]);
	}
	free(lines);
}

/*
** Recursively free a BSP node and all its children
** Safe to call with NULL pointer
*/
void	free_bsp_node(t_bsp_node *node)
{
	if (!node)
		return ;
	if (node->front)
		free_bsp_node(node->front);
	if (node->back)
		free_bsp_node(node->back);
	if (node->partition)
		free(node->partition);
	free_node_lines(node->lines, node->num_lines);
	free(node);
}

/*
** Copy lines from old array to new array
** Also frees the old array
** Returns true on successful copy
*/
static bool	copy_old_lines(t_bsp_line **new, t_bsp_line **old, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		new[i] = old[i];
	free(old);
	return (true);
}

/*
** Add line to node's line array
** Returns false if reallocation fails
*/
bool	add_line_to_node(t_bsp_node *node, t_bsp_line *line)
{
	t_bsp_line	**new_lines;

	new_lines = malloc(sizeof(t_bsp_line *) * (node->num_lines + 1));
	if (!new_lines)
		return (false);
	if (node->lines && !copy_old_lines(new_lines, node->lines, node->num_lines))
	{
		free(new_lines);
		return (false);
	}
	new_lines[node->num_lines] = line;
	node->lines = new_lines;
	node->num_lines++;
	return (true);
}

/*
** Copy all lines except one specific line
** New array should have space for num-1 elements
** Skips line matching exc parameter
*/
static void	copy_lines_except(t_bsp_line **new, t_bsp_line **old,
		t_bsp_line *exc, int num)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < num)
	{
		if (old[i] != exc)
			new[j++] = old[i];
	}
}

/*
** Remove line from node's line array
** Returns false if line not found
*/
bool	remove_line_from_node(t_bsp_node *node, t_bsp_line *line)
{
	t_bsp_line	**new_lines;

	if (!node->lines || node->num_lines == 0)
		return (false);
	new_lines = malloc(sizeof(t_bsp_line *) * (node->num_lines - 1));
	if (!new_lines)
		return (false);
	copy_lines_except(new_lines, node->lines, line, node->num_lines);
	if (ft_memcmp(new_lines, node->lines, sizeof(t_bsp_line *) * 
		(node->num_lines - 1)) == 0)
	{
		free(new_lines);
		return (false);
	}
	free(node->lines);
	node->lines = new_lines;
	node->num_lines--;
	return (true);
}

/*
** Free entire BSP tree and all its nodes
** Safe to call with NULL pointer
*/
void	free_bsp_tree(t_bsp_tree *tree)
{
	if (!tree)
		return ;
	free_bsp_node(tree->root);
	free(tree);
}
