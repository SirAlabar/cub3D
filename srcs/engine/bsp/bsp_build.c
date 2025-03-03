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
#include<colors.h>

t_fixed32 calculate_lines_size(t_bsp_line **lines, int num_lines)
{
    t_fixed32 total_size = 0;
    int i;

	i = -1;
    while (++i < num_lines)
    {
        total_size += fixed32_vec_dist(lines[i]->start, lines[i]->end);
    }
    return (total_size);
}
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
// t_fixed32 eval_partition_quality(t_bsp_line *partition, t_bsp_line **lines, 
// 	int num_lines, int depth)
// {
// 	t_count_data count;
// 	t_fixed32 balance_score;
// 	t_fixed32 split_score;
// 	t_fixed32 depth_penalty;
// 	int i;

// 	init_count_data(&count);
// 	count.tree_depth = depth;

// 	i = -1;
// 	while (++i < num_lines)
// 	{
// 		if (lines[i] != partition)
// 			count_line_sides(lines[i], partition, &count);
// 	}

// 	balance_score = int_to_fixed32(abs(count.front - count.back));
// 	split_score = int_to_fixed32(count.split * SPLIT_PENALTY);
// 	depth_penalty = int_to_fixed32((depth * depth) / BSP_MAX_DEPTH);

// 	return fixed32_add(fixed32_add(balance_score, split_score), depth_penalty);
// }

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
// t_bsp_node	*build_subtrees(t_bsp_node *node, t_bsp_data *data)
// {
// 	if (!node || !data)
// 		return (NULL);
// 	if (data->num_front > 0)
// 		node->front = build_bsp_tree(data->front_lines, data->num_front,
// 				node->depth + 1);
// 	else
// 		node->front = NULL;
// 	if (data->num_back > 0)
// 		node->back = build_bsp_tree(data->back_lines, data->num_back,
// 				node->depth + 1);
// 	else
// 		node->back = NULL;
// 	free(data->front_lines);
// 	free(data->back_lines);
// 	return (node);
// }

// t_bsp_node *build_subtrees(t_bsp_node *node, t_bsp_data *data)
// {
//     if (!node || !data)
//     {
//         ft_printf("ERROR: NULL node or data in build_subtrees\n");
//         return (NULL);
//     }

//     // Construção do nó front
//     if (data->num_front > 0)
//     {
//         ft_printf("Building front subtree with %d lines\n", data->num_front);
        
//         // Verifica validade das linhas front
//         for (int i = 0; i < data->num_front; i++)
//         {
//             if (!data->front_lines[i])
//             {
//                 ft_printf("ERROR: NULL front line at index %d\n", i);
//                 data->num_front = i;  // Trunca linhas inválidas
//                 break;
//             }
//         }
        
//         node->front = build_bsp_tree(data->front_lines, data->num_front, 
//                                      node->depth + 1);
//     }
//     else
//     {
//         ft_printf("No front lines\n");
//         node->front = NULL;
//     }

//     // Construção do nó back (similar ao front)
//     if (data->num_back > 0)
//     {
//         ft_printf("Building back subtree with %d lines\n", data->num_back);
        
//         // Verifica validade das linhas back
//         for (int i = 0; i < data->num_back; i++)
//         {
//             if (!data->back_lines[i])
//             {
//                 ft_printf("ERROR: NULL back line at index %d\n", i);
//                 data->num_back = i;  // Trunca linhas inválidas
//                 break;
//             }
//         }
        
//         node->back = build_bsp_tree(data->back_lines, data->num_back, 
//                                     node->depth + 1);
//     }
//     else
//     {
//         ft_printf("No back lines\n");
//         node->back = NULL;
//     }

//     // Libera arrays de linhas
//     free(data->front_lines);
//     free(data->back_lines);

//     return (node);
// }
/*
** Builds BSP tree recursively with depth tracking
** Uses balanced partition selection at each level
** Returns NULL if max depth reached or build fails
*/
// t_bsp_node *build_bsp_tree(t_bsp_line **lines, int num_lines, int depth)
// {
//     t_bsp_node *node;
//     t_bsp_data split_data;

// 	ft_printf("Building BSP tree node at depth %d with %d lines\n", depth, num_lines);
//     if (!lines || num_lines <= 0)
//         return (NULL);
//     node = create_bsp_node();
//     if (!node)
//         return (NULL);
//     node->depth = depth;
//     if (depth >= BSP_MAX_DEPTH || num_lines <= 4 ||
//         calculate_lines_size(lines, num_lines) <= BSP_MIN_NODE_SIZE)
//     {
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }
//     node->partition = choose_partition(lines, num_lines);
//     if (!node->partition)
//     {
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }
//     if (!split_lines(node->partition, lines, num_lines, &split_data))
//     {
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }
//     node = build_subtrees(node, &split_data);
//     if (!node)
//     {
//         node = create_bsp_node();
//         node->lines = lines;
//         node->num_lines = num_lines;
//     }
//     return (node);
// }

// t_bsp_node *build_bsp_tree(t_bsp_line **lines, int num_lines, int depth)
// {
//     t_bsp_node *node;
//     t_bsp_data split_data;

//     // Validações iniciais de segurança
//     if (!lines)
//     {
//         ft_printf("ERROR: NULL lines pointer in build_bsp_tree\n");
//         return (NULL);
//     }

//     if (num_lines <= 0)
//     {
//         ft_printf("ERROR: Invalid number of lines (%d)\n", num_lines);
//         return (NULL);
//     }

//     // Verifica se as linhas são válidas
//     for (int i = 0; i < num_lines; i++)
//     {
//         if (!lines[i])
//         {
//             ft_printf("ERROR: NULL line pointer at index %d\n", i);
//             return (NULL);
//         }
//     }

//     node = create_bsp_node();
//     if (!node)
//     {
//         ft_printf("ERROR: Failed to create BSP node\n");
//         return (NULL);
//     }

//     node->depth = depth;

//     // Condições para nó folha
//     if (depth >= BSP_MAX_DEPTH || num_lines <= 4 ||
//         calculate_lines_size(lines, num_lines) <= BSP_MIN_NODE_SIZE)
//     {
//         ft_printf("Creating leaf node at depth %d with %d lines\n", depth, num_lines);
        
//         // Atribuição direta dos ponteiros
//         node->lines = lines;
//         node->num_lines = num_lines;
        
//         return (node);
//     }

//     node->partition = choose_partition(lines, num_lines);
//     if (!node->partition)
//     {
//         ft_printf("No partition found, creating leaf node\n");
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }

//     // Debug para partição
//     ft_printf("Partition line: (%d,%d) -> (%d,%d)\n", 
//            fixed32_to_int(node->partition->start.x),
//            fixed32_to_int(node->partition->start.y),
//            fixed32_to_int(node->partition->end.x),
//            fixed32_to_int(node->partition->end.y));

//     // Tenta dividir as linhas
//     if (!split_lines(node->partition, lines, num_lines, &split_data))
//     {
//         ft_printf("Line split failed, creating leaf node\n");
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }

//     // Debug para linhas divididas
//     ft_printf("Front lines: %d, Back lines: %d\n", 
//            split_data.num_front, split_data.num_back);

//     // Constrói subárvores
//     return build_subtrees(node, &split_data);
// }

// t_bsp_node *build_bsp_tree(t_bsp_line **lines, int num_lines, int depth)
// {
//     t_bsp_node *node;
//     t_bsp_data split_data;

//     if (!lines || num_lines <= 0)
//         return (NULL);
//     node = create_bsp_node();
//     if (!node)
//         return (NULL);
//     node->depth = depth;
// 	ft_printf("depth %d, linesize %d\n", depth, fixed32_abs(calculate_lines_size(lines, num_lines)));
// 	if (depth >= BSP_MAX_DEPTH)
//     {
//         ft_printf("===========================\nCreating leaf node at depth %d with %d lines====================\n", depth, num_lines);
        
//         // Atribuição direta dos ponteiros
//         node->lines = lines;
//         node->num_lines = num_lines;
        
//         return (node);
//     }
//     node->partition = choose_partition(lines, num_lines);
//     if (!node->partition)
//     {
//         node->lines = lines;
//         node->num_lines = num_lines;
//         return (node);
//     }
//     if (!split_lines(node->partition, lines, num_lines, &split_data))
//     {
//         free(node);
//         return (NULL);
//     }
	
// 	ft_printf("Node at depth %d -> Front: %d lines, Back: %d lines\n", 
// 		node->depth, split_data.num_front, split_data.num_back);
//     return (build_subtrees(node, &split_data));
// }

// static t_bsp_node	*build_back_subtree(t_bsp_node *node, t_bsp_data *data)
// {
// 	int	i;

// 	if (data->num_back > 0)
// 	{
// 		ft_printf("Building back subtree with %d lines\n", data->num_back);
// 		i = 0;
// 		while (i < data->num_back)
// 		{
// 			if (!data->back_lines[i])
// 			{
// 				ft_printf("ERROR: NULL back line at index %d\n", i);
// 				data->num_back = i;
// 				break ;
// 			}
// 			i++;
// 		}
// 		return (build_bsp_tree(data->back_lines, data->num_back,
// 				node->depth + 1));
// 	}
// 	ft_printf("No back lines\n");
// 	return (NULL);
// }

// static t_bsp_node	*build_front_subtree(t_bsp_node *node, t_bsp_data *data)
// {
// 	int	i;

// 	if (data->num_front > 0)
// 	{
// 		ft_printf("Building front subtree with %d lines\n", data->num_front);
// 		i = 0;
// 		while (i < data->num_front)
// 		{
// 			if (!data->front_lines[i])
// 			{
// 				ft_printf("ERROR: NULL front line at index %d\n", i);
// 				data->num_front = i;
// 				break ;
// 			}
// 			i++;
// 		}
// 		return (build_bsp_tree(data->front_lines, data->num_front,
// 				node->depth + 1));
// 	}
// 	ft_printf("No front lines\n");
// 	return (NULL);
// }

t_bsp_node *build_subtrees(t_bsp_node *node, t_bsp_data *data, t_thread_pool *pool)
{
	if (!node || !data)
	{
		ft_printf(RED"ERROR: NULL node or data in build_subtrees\n"DEFAULT);
		return (NULL);
	}

	// Construir o nó frontal se houver linhas
	if (data->num_front > 0)
	{
		ft_printf(CYAN"Building front subtree with %d lines\n"DEFAULT, data->num_front);
		// Verificação de linhas nulas para debug
		for (int i = 0; i < data->num_front; i++)
		{
			if (!data->front_lines[i])
			{
				ft_printf(RED"Warning: NULL front line at index %d\n"DEFAULT, i);
			}
		}
		node->front = build_bsp_tree(data->front_lines, data->num_front, node->depth + 1, pool);
	}
	else
	{
		ft_printf("No front lines\n");
		node->front = NULL;
	}

	// Construir o nó traseiro se houver linhas
	if (data->num_back > 0)
	{
		ft_printf(CYAN"Building back subtree with %d lines\n"DEFAULT, data->num_back);
		// Verificação de linhas nulas para debug
		for (int i = 0; i < data->num_back; i++)
		{
			if (!data->back_lines[i])
			{
				ft_printf(RED"Warning: NULL back line at index %d\n"DEFAULT, i);
			}
		}
		node->back = build_bsp_tree(data->back_lines, data->num_back, node->depth + 1, pool);
	}
	else
	{
		ft_printf("No back lines\n");
		node->back = NULL;
	}

	// Libera arrays temporários mantendo os dados nos nós
	free(data->front_lines);
	free(data->back_lines);
	
	return (node);
}

t_bsp_node *build_bsp_tree(t_bsp_line **lines, int num_lines, int depth, t_thread_pool *pool)
{
	t_bsp_node	*node;
	t_bsp_data	split_data;

	if (!lines || num_lines <= 0)
		return (NULL);
	node = create_bsp_node();
	if (!node)
		return (NULL);
	node->depth = depth;
	ft_printf("depth %d, linesize %d\n", depth,
		fixed32_abs(calculate_lines_size(lines, num_lines)));
	if (depth >= BSP_MAX_DEPTH)
    {
        node->lines = lines;
        node->num_lines = num_lines;
        return (node);
    }
    node->partition = choose_partition(lines, num_lines, pool);
	if (!node->partition)
	{
		node->lines = lines;
		node->num_lines = num_lines;
		return (node);
	}
	if (!split_lines(node->partition, lines, num_lines, &split_data))
	{
		ft_printf(RED"Split operation failed, falling back to leaf node\n"DEFAULT);
		node->lines = lines;
		node->num_lines = num_lines;
		// Falha ao dividir não deve ser fatal - cria nó folha
		return (node);
	}
	node->lines = lines;
	node->num_lines = num_lines;
	if (!build_subtrees(node, &split_data, pool))
	{
		ft_printf(RED"Failed to build subtrees\n"DEFAULT);
		// Já temos as linhas atribuídas, apenas retorna o nó
		return (node);
	}
	return (node);
}
