/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_traverse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Find node containing given point in BSP tree
** Returns NULL if point not found or tree is empty
*/
t_bsp_node *find_node(t_bsp_node *node, t_fixed_vec32 point)
{
    if (!node)
        return (NULL);

    static int max_depth = 10;
    static int current_depth = 0;

    if (current_depth >= max_depth)
    {
        ft_printf("Max depth reached\n");
        return (NULL);
    }


    if (!node->partition)
    {
        ft_printf("Reached leaf node\n");
        return (node);
    }

    // Adicionar diagnóstico detalhado
    ft_printf("Node partition: (%d,%d) -> (%d,%d)\n", 
              fixed32_to_int(node->partition->start.x),
              fixed32_to_int(node->partition->start.y),
              fixed32_to_int(node->partition->end.x),
              fixed32_to_int(node->partition->end.y));

    t_bsp_side side = bsp_classify_point(point, node->partition);
    
    current_depth++;
    t_bsp_node *result = NULL;

    switch (side)
    {
        case BSP_FRONT:
            if (node->front)
                result = find_node(node->front, point);
            break;
        case BSP_BACK:
            if (node->back)
                result = find_node(node->back, point);
            break;
        case BSP_COLINEAR:
        case BSP_SPANNING:
            if (node->front)
                result = find_node(node->front, point);
            if (!result && node->back)
                result = find_node(node->back, point);
            break;
    }

    current_depth--;
    return result;
}

/*
** Traverse BSP tree front-to-back from given viewpoint
** Calls process_node for each node in correct order
*/
void	traverse_front_to_back(t_bsp_node *node, t_fixed_vec32 viewpoint,
		void (*process_node)(t_bsp_node *))
{
	t_bsp_side	side;

	if (!node || !process_node)
		return ;
	if (!node->partition)
	{
		process_node(node);
		return ;
	}
	side = bsp_classify_point(viewpoint, node->partition);
	if (side == BSP_FRONT)
	{
		traverse_front_to_back(node->back, viewpoint, process_node);
		process_node(node);
		traverse_front_to_back(node->front, viewpoint, process_node);
	}
	else
	{
		traverse_front_to_back(node->front, viewpoint, process_node);
		process_node(node);
		traverse_front_to_back(node->back, viewpoint, process_node);
	}
}

/*
** Process only nodes in front of partition
** Used for optimization in specific cases
*/
void	traverse_front_nodes(t_bsp_node *node, t_fixed_vec32 viewpoint,
		void (*process_node)(t_bsp_node *))
{
	if (!node || !process_node)
		return ;
	if (!node->partition)
	{
		process_node(node);
		return ;
	}
	if (bsp_classify_point(viewpoint, node->partition) == BSP_FRONT)
	{
		process_node(node);
		traverse_front_nodes(node->front, viewpoint, process_node);
	}
}

/*
** Count number of nodes in front of viewpoint
** Used for statistics and debugging
*/
int	count_front_nodes(t_bsp_node *node, t_fixed_vec32 viewpoint)
{
	int	count;

	if (!node)
		return (0);
	count = 0;
	if (!node->partition)
		return (1);
	if (bsp_classify_point(viewpoint, node->partition) == BSP_FRONT)
	{
		count = 1;
		count += count_front_nodes(node->front, viewpoint);
	}
	return (count);
}
