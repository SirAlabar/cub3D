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
#include <colors.h>

/*
** Find node containing given point in BSP tree
** Returns NULL if point not found or tree is empty
*/
t_bsp_node *find_node(t_bsp_node *node, t_fixed_vec32 point)
{
    t_bsp_side side;
    
    if (!node)
        return (NULL);

    // Se é um nó folha, retorna ele mesmo
    if (!node->partition)
    {
        ft_printf(GREEN"Found leaf node\n"DEFAULT);
        return (node);
    }

    // Debug da partição atual
    ft_printf("Current partition: (%d,%d) -> (%d,%d)\n",
        fixed32_to_int(node->partition->start.x),
        fixed32_to_int(node->partition->start.y),
        fixed32_to_int(node->partition->end.x),
        fixed32_to_int(node->partition->end.y));

    // Classifica o ponto em relação à partição
    side = bsp_classify_point(point, node->partition);

    if (side == BSP_COLINEAR || side == BSP_FRONT)
    {
        if (node->front)
            return find_node(node->front, point);
        return node;
    }
    else
    {
        if (node->back)
            return find_node(node->back, point);
        return node;
    }
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
