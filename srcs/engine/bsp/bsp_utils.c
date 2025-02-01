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
** Initializes BSP tree construction
** Focuses solely on BSP creation from map lines
*/

t_bsp_tree	*init_bsp_build(t_game *game)
{
	t_bsp_tree	*tree;
	t_bsp_line	**lines;
	int			num_lines;

	tree = malloc(sizeof(t_bsp_tree));
	if (!tree)
		return (NULL);
	if (!extract_map_lines(game, &lines, &num_lines))
	{
		free(tree);
		return (NULL);
	}
	tree->root = build_bsp_tree(lines, num_lines);
	free(lines);
	if (!tree->root)
	{
		free(tree);
		return (NULL);
	}
	return (tree);
}

/*
** Creates a BSP line from two points in fixed-point format
** @param start_x: starting x coordinate
** @param start_y: starting y coordinate
** @param end_x: ending x coordinate
** @param end_y: ending y coordinate
** @return: newly allocated BSP line, or NULL if allocation fails
*/
static t_bsp_line	*create_bsp_line(int start_x, int start_y, int end_x,
		int end_y)
{
	t_bsp_line	*line;

	line = (t_bsp_line *)malloc(sizeof(t_bsp_line));
	if (!line)
		return (NULL);
	line->start.x = int_to_fixed32(start_x);
	line->start.y = int_to_fixed32(start_y);
	line->end.x = int_to_fixed32(end_x);
	line->end.y = int_to_fixed32(end_y);
	return (line);
}

/*
** Creates and initializes a new BSP node
** Sets all pointers to NULL and initializes other values to defaults
** @return: newly allocated and initialized node, or NULL if allocation fails
*/
t_bsp_node	*create_bsp_node(void)
{
	t_bsp_node	*node;

	node = (t_bsp_node *)ft_calloc(1, sizeof(t_bsp_node));
	if (!node)
		return (NULL);
	node->front = NULL;
	node->back = NULL;
	node->line = NULL;
	node->num_lines = 0;
	node->lines = NULL;
	return (node);
}

/*
** Utility function to free memory and return a value
** Helps avoid memory leaks when returning early from functions
** @param ptr: pointer to memory to free
** @param ret: value to return
** @return: the ret value passed in
*/
void	*free_and_return(void *ptr, void *ret)
{
	if (ptr)
		free(ptr);
	return (ret);
}
