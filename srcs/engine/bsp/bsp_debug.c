/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Print line details in fixed-point format
** Shows start, end points and type
    === BSP Tree ===

    Partition: (10,20) -> (30,40) [type: 0]
    Lines (2):
    [0] Line: (5,15) -> (25,35) [type: 0]
    [1] Line: (15,25) -> (35,45) [type: 1]
    ├── Front:
    │   Partition: (15,25) -> (35,45) [type: 1]
    │   Lines (1):
    │     [0] Line: (20,30) -> (40,50) [type: 0]
    │   └── Back:
    │       Leaf Node
    │       Lines (0):
    └── Back:
        Partition: (5,15) -> (25,35) [type: 0]
        Lines (1):
        [0] Line: (0,10) -> (20,30) [type: 1]
        └── Front:
            Leaf Node
            Lines (0):

    === End of Tree ===
*/
static void	print_line_info(t_bsp_line *line, int depth)
{
	int	i;

	i = -1;
	while (++i < depth)
		ft_printf("  ");
	ft_printf("Line: (%d,%d) -> (%d,%d) [type: %d]\n",
		fixed32_to_int(line->start.x),
		fixed32_to_int(line->start.y),
		fixed32_to_int(line->end.x),
		fixed32_to_int(line->end.y),
		line->type);
}

/*
** Print all lines in a node
*/
static void	print_node_lines(t_bsp_node *node, int depth)
{
	int	i;
	int	j;

	i = -1;
	while (++i < depth)
		ft_printf("  ");
	ft_printf("Lines (%d):\n", node->num_lines);
	i = -1;
	while (++i < node->num_lines)
	{
		j = -1;
		while (++j < depth + 1)
			ft_printf("  ");
		ft_printf("[%d] ", i);
		print_line_info(node->lines[i], 0);
	}
}

/*
** Print partition info of a node
*/
static void	print_partition_info(t_bsp_node *node, int depth)
{
	int	i;

	i = -1;
	while (++i < depth)
		ft_printf("  ");
	if (node->partition)
	{
		ft_printf("Partition: ");
		print_line_info(node->partition, 0);
	}
	else
		ft_printf("Leaf Node\n");
}

/*
** Print tree structure recursively with visual guides
*/
void	print_bsp_tree_recursive(t_bsp_node *node, int depth, char *prefix)
{
	char	*new_prefix;
	int		prefix_len;

	if (!node)
		return ;
	ft_printf("%s\n", prefix);
	print_partition_info(node, depth);
	print_node_lines(node, depth);
	prefix_len = ft_strlen(prefix);
	new_prefix = malloc(prefix_len + 4);
	if (!new_prefix)
		return ;
	ft_strlcpy(new_prefix, prefix, prefix_len + 1);
	if (node->front)
	{
		ft_printf("%s├── Front:\n", prefix);
		ft_strlcat(new_prefix, "│   ", prefix_len + 4);
		print_bsp_tree_recursive(node->front, depth + 1, new_prefix);
	}
	if (node->back)
	{
		ft_printf("%s└── Back:\n", prefix);
		ft_strlcpy(new_prefix + prefix_len, "    ", 4);
		print_bsp_tree_recursive(node->back, depth + 1, new_prefix);
	}
	free(new_prefix);
}

/*
** Main function to print entire BSP tree
** Includes tree statistics and formatting
*/
void	print_bsp_tree(t_bsp_tree *tree)
{
	ft_printf("\n=== BSP Tree ===\n\n");
	if (!tree || !tree->root)
	{
		ft_printf("Empty tree\n");
		return ;
	}
	print_bsp_tree_recursive(tree->root, 0, "");
	ft_printf("\n=== End of Tree ===\n");
}
