/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Check if line has valid geometry (non-zero length)
** Returns false for degenerate lines
*/
static bool	validate_line_geometry(t_bsp_line *line)
{
	t_fixed_vec32	delta;
	t_fixed32		length_sq;

	if (!line)
		return (false);
	delta.x = fixed32_sub(line->end.x, line->start.x);
	delta.y = fixed32_sub(line->end.y, line->start.y);
	length_sq = fixed32_add(fixed32_mul(delta.x, delta.x),
			fixed32_mul(delta.y, delta.y));
	return (length_sq > 0);
}

/*
** Check node for geometry and partition issues
** Returns number of errors found (0 if valid)
*/
static int	validate_node_geometry(t_bsp_node *node)
{
	int	i;
	int	errors;

	if (!node)
		return (0);
	errors = 0;
	if (node->partition && !validate_line_geometry(node->partition))
	{
		ft_printf("Error: Invalid partition geometry\n");
		errors++;
	}
	i = -1;
	while (++i < node->num_lines)
	{
		if (!validate_line_geometry(node->lines[i]))
		{
			ft_printf("Error: Invalid line geometry at index %d\n", i);
			errors++;
		}
	}
	return (errors);
}

/*
** Validate entire BSP tree recursively
** Returns total number of errors found
*/
static int	validate_bsp_tree_recursive(t_bsp_node *node, int depth,
		int *max_depth)
{
	int	errors;

	if (!node)
		return (0);
	if (depth > *max_depth)
		*max_depth = depth;
	errors = validate_node_geometry(node);
	if (node->front)
		errors += validate_bsp_tree_recursive(node->front, depth + 1, max_depth);
	if (node->back)
		errors += validate_bsp_tree_recursive(node->back, depth + 1, max_depth);
	return (errors);
}

/*
** Main validation function for BSP tree
** Returns true if tree is valid
*/
bool	validate_bsp_tree(t_bsp_tree *tree)
{
	int	errors;
	int	max_depth;

	if (!tree || !tree->root)
	{
		ft_printf("Error: Invalid BSP tree structure\n");
		return (false);
	}
	max_depth = 0;
	errors = validate_bsp_tree_recursive(tree->root, 0, &max_depth);
	ft_printf("BSP Tree Stats:\n");
	ft_printf("- Max Depth: %d\n", max_depth);
	ft_printf("- Total Errors: %d\n", errors);
	return (errors == 0);
}

/*
** Print detailed information about a BSP node
** Used for debugging tree structure
*/
void	debug_print_node(t_bsp_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		ft_printf("  ");
	ft_printf("Node at depth %d:\n", depth);
	i = -1;
	while (++i < depth)
		ft_printf("  ");
	ft_printf("- Lines: %d\n", node->num_lines);
	if (node->front)
		debug_print_node(node->front, depth + 1);
	if (node->back)
		debug_print_node(node->back, depth + 1);
}
