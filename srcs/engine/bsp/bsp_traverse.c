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

#include <cub3d.h>

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
int count_front_nodes(t_bsp_node *node, t_fixed_vec32 viewpoint)
{
    if (!node)
        return (0);

    // Nó folha sempre conta
    if (!node->partition)
        return (1);

    // Classifica o ponto em relação à partição
    t_bsp_side side = bsp_classify_point(viewpoint, node->partition);

    // Debug mais detalhado
    ft_printf("Side classification: %d\n", side);
    ft_printf("BSP_FRONT: %d, BSP_BACK: %d, BSP_COLINEAR: %d, BSP_SPANNING: %d\n", 
        BSP_FRONT, BSP_BACK, BSP_COLINEAR, BSP_SPANNING);

    // Considera COLINEAR como FRONT
    if (side == BSP_FRONT || side == BSP_COLINEAR)
    {
        int count = 1;  // Conta o nó atual
        
        // Adiciona contagem dos filhos na frente
        if (node->front)
            count += count_front_nodes(node->front, viewpoint);
        
        return count;
    }

    // Se não está na frente, verifica filho de trás
    if (node->back)
        return count_front_nodes(node->back, viewpoint);

    return 0;
}

bool	is_segment_visible(t_fixed_vec32 v1, t_fixed_vec32 v2)
{
	t_fixed32	angle1;
	t_fixed32	angle2;
	t_fixed32	half_fov;

	// Se ambos pontos estão atrás da câmera, o segmento não é visível
	if (v1.y <= 0 && v2.y <= 0)
		return (false);
	
	// Se um ponto está atrás da câmera, precisamos recortar (simplificado)
	if (v1.y <= 0 || v2.y <= 0)
		return (true);
	
	// Verificação do campo de visão horizontal
	half_fov = bam_to_fixed32(FOV >> 1);
	angle1 = fixed32_mul(v1.y, fixed32_tan(FOV >> 1));
	angle2 = fixed32_mul(v2.y, fixed32_tan(FOV >> 1));
	
	// Se ambos ângulos estão fora do FOV no mesmo lado, o segmento não é visível
	if ((angle1 > half_fov && angle2 > half_fov) || 
		(angle1 < -half_fov && angle2 < -half_fov))
		return (false);
	
	return (true);
}

bool is_line_in_fov(t_game *game, t_bsp_line *line)
{
    // ft_printf("\nChecking line in FOV:\n");
    // ft_printf("Player position: (%d, %d)\n", 
    //     fixed32_to_int(game->p1.pos.x), 
    //     fixed32_to_int(game->p1.pos.y));
    // ft_printf("Player angle: %d\n", game->p1.angle);
    
    // ft_printf("Line start: (%d, %d)\n", 
    //     fixed32_to_int(line->start.x), 
    //     fixed32_to_int(line->start.y));
    // ft_printf("Line end: (%d, %d)\n", 
    //     fixed32_to_int(line->end.x), 
    //     fixed32_to_int(line->end.y));

    // Transforma os pontos da linha para espaço de visão
    t_fixed_vec32 v1 = transform_point(line->start, game);
    t_fixed_vec32 v2 = transform_point(line->end, game);

    // ft_printf("Transformed points:\n");
    // ft_printf("  v1: (%d, %d)\n", 
    //     fixed32_to_int(v1.x), fixed32_to_int(v1.y));
    // ft_printf("  v2: (%d, %d)\n", 
    //     fixed32_to_int(v2.x), fixed32_to_int(v2.y));

    // Usa a função de visibilidade existente
    bool visible = is_segment_visible(v1, v2);
    
    ft_printf("Visibility result: %s\n", visible ? "VISIBLE" : "HIDDEN");

    return visible;
}


// Função para encontrar linha por linedef_index
t_bsp_line *find_line_by_linedef_index(t_bsp_node *node, int linedef_index)
{
    if (!node)
        return NULL;

    // Procura na lista de linhas do nó
    for (int i = 0; i < node->num_lines; i++)
    {
        if (node->lines[i] && node->lines[i]->linedef_index == linedef_index)
        {
            return node->lines[i];
        }
    }

    // Recursivamente procura nos filhos
    t_bsp_line *line;
    
    // Procura no filho da frente
    if (node->front)
    {
        line = find_line_by_linedef_index(node->front, linedef_index);
        if (line)
            return line;
    }

    // Procura no filho de trás
    if (node->back)
    {
        line = find_line_by_linedef_index(node->back, linedef_index);
        if (line)
            return line;
    }

    return NULL;
}

void	mark_line_if_visible(t_bsp_line *line, t_game *game,
	t_visible_lines *visible_lines)
{
	bool	is_visible;

	if (!line)
		return ;
	is_visible = is_line_in_fov(game, line);
	ft_printf("  Visibility: %s\n", is_visible ? "VISIBLE" : "HIDDEN");
	if (is_visible && visible_lines->count < MAX_LINEDEFS)
		visible_lines->line_ids[visible_lines->count++] = line->linedef_index;
}

void	mark_lines_in_node(t_bsp_node *node, t_game *game,
	t_visible_lines *visible_lines)
{
	int	i;

	i = -1;
	while (++i < node->num_lines)
		mark_line_if_visible(node->lines[i], game, visible_lines);
}


void	mark_visible_bsp_lines(t_bsp_node *node,
	t_game *game, t_visible_lines *visible_lines)
{
	int			i;
	t_bsp_side	side;

	if (!node)
		return ;
	ft_printf("\nProcessing node %p\n", (void *)node);
	ft_printf("Number of lines in node: %d\n", node->num_lines);
	if (!node->partition)
	{
		ft_printf("Leaf node detected\n");
		i = -1;
		while (++i < node->num_lines)
			mark_line_if_visible(node->lines[i], game, visible_lines);
		return ;
	}
	side = bsp_classify_point(game->p1.pos, node->partition);
	ft_printf("Partition side classification: %d\n", side);
	if (side == BSP_FRONT || side == BSP_COLINEAR)
	{
		mark_visible_bsp_lines(node->back, game, visible_lines);
		mark_lines_in_node(node, game, visible_lines);
		mark_visible_bsp_lines(node->front, game, visible_lines);
	}
	else
	{
		mark_visible_bsp_lines(node->front, game, visible_lines);
		mark_lines_in_node(node, game, visible_lines);
		mark_visible_bsp_lines(node->back, game, visible_lines);
	}
}
