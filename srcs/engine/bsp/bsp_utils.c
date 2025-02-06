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

t_bsp_tree *init_bsp_build(t_doom_map *map)
{
    t_bsp_tree *tree;
    t_bsp_line **lines;
    int num_lines;

    tree = ft_calloc(1, sizeof(t_bsp_tree));
    if (!tree)
        return (NULL);
    if (!extract_map_lines(map, &lines, &num_lines))
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
** Calculates normalized normal vector for a BSP line
** Normal points to the left side of the line when facing from start to end
** Returns normalized normal vector
*/
static t_fixed_vec32	calculate_normal(t_fixed_vec32 start, t_fixed_vec32 end)
{
	t_fixed_vec32	delta;
	t_fixed_vec32	normal;
	t_fixed32		length;

	delta.x = fixed32_sub(end.x, start.x);
	delta.y = fixed32_sub(end.y, start.y);
	length = fixed32_sqrt(fixed32_add(fixed32_mul(delta.x, delta.x),
				fixed32_mul(delta.y, delta.y)));
	if (length == 0)
		return ((t_fixed_vec32){0, 0});
	normal.x = fixed32_div(-delta.y, length);
	normal.y = fixed32_div(delta.x, length);
	return (normal);
}

/*
** Creates a new BSP line with given parameters
** Calculates and stores the normal vector
** Returns NULL if allocation fails
*/
t_bsp_line	*create_bsp_line(t_fixed_vec32 start, t_fixed_vec32 end, int type)
{
	t_bsp_line	*line;

	line = ft_calloc(1, sizeof(t_bsp_line));
	if (!line)
		return (NULL);
	line->start = start;
	line->end = end;
	line->type = type;
	line->normal = calculate_normal(start, end);
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
	node->lines = NULL;
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
