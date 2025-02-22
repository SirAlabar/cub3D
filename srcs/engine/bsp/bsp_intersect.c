/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_intersect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>
# include <colors.h>

/*
** Tests intersection between two line segments using parametric equations
** Uses small epsilon for precise intersection testing at line endpoints
** Returns true if segments intersect at a non-endpoint point
*/
static bool	line_intersect(t_fixed_vec32 p1, t_fixed_vec32 p2, t_bsp_line *wall)
{
	t_fixed_vec32	v1;
	t_fixed_vec32	v2;
	t_fixed32		det;
	t_fixed32		t;
	t_fixed32		u;

	v1.x = fixed32_sub(p2.x, p1.x);
	v1.y = fixed32_sub(p2.y, p1.y);
	v2.x = fixed32_sub(wall->end.x, wall->start.x);
	v2.y = fixed32_sub(wall->end.y, wall->start.y);
	det = fixed32_sub(fixed32_mul(v1.x, v2.y), fixed32_mul(v1.y, v2.x));
	
	ft_printf(BLUE"Line vectors: v1(%d,%d) v2(%d,%d)\n"DEFAULT, 
		v1.x, v1.y, v2.x, v2.y);
	ft_printf(BLUE"Determinant: %d\n"DEFAULT, det);
	
	if (fixed32_abs(det) < (FIXED32_HALF >> 10))
		return (false);
	
	t = fixed32_div(fixed32_sub(fixed32_mul(v2.x, fixed32_sub(p1.y,
					wall->start.y)), fixed32_mul(v2.y, fixed32_sub(p1.x,
					wall->start.x))), det);
	u = fixed32_div(fixed32_sub(fixed32_mul(v1.x, fixed32_sub(p1.y,
					wall->start.y)), fixed32_mul(v1.y, fixed32_sub(p1.x,
					wall->start.x))), det);
					
	ft_printf(YELLOW"Intersection parameters: t=%d u=%d\n"DEFAULT, t, u);
	return ((t > (FIXED32_HALF >> 8) && t < (FIXED_POINT_SCALE - (FIXED32_HALF >> 8)))
		&& (u > (FIXED32_HALF >> 8) && u < (FIXED_POINT_SCALE - (FIXED32_HALF >> 8))));
}

/*
** Finds closest wall to a point in current BSP node
** Uses wall normals for efficient distance calculation
** Returns actual fixed-point distance, not scaled
*/
t_fixed32	find_nearest_wall(t_bsp_node *node, t_fixed_vec32 point)
{
	t_fixed32		min_dist;
	t_fixed32		curr_dist;
	t_fixed_vec32	delta;
	int				i;

	if (!node || !node->num_lines)
		return (-1);
	min_dist = INT32_MAX;
	i = -1;
	while (++i < node->num_lines)
	{
		delta.x = fixed32_sub(point.x, node->lines[i]->start.x);
		delta.y = fixed32_sub(point.y, node->lines[i]->start.y);
		curr_dist = fixed32_abs(fixed32_add(fixed32_mul(
						node->lines[i]->normal.x, delta.x),
					fixed32_mul(node->lines[i]->normal.y, delta.y)));
		
		ft_printf(GREEN"Wall %d: delta(%d,%d) dist=%d\n"DEFAULT, 
			i, delta.x, delta.y, curr_dist);
		
		if (curr_dist < min_dist)
			min_dist = curr_dist;
	}
	ft_printf(RED"Nearest wall distance: %d\n"DEFAULT, min_dist);
	return (min_dist);
}

/*
** Validates movement between points for wall collisions
** Checks both distance to walls and path intersection
** Returns true if movement is valid with no collisions
*/
bool check_movement_valid(t_bsp_tree *tree, t_fixed_vec32 start, t_fixed_vec32 end)
{
    t_bsp_node *start_node;
    t_bsp_node *end_node;
    t_fixed32 move_dist;
    int i;

    ft_printf(CYAN"\nMovement check: (%d,%d) -> (%d,%d)\n"DEFAULT,
        fixed32_to_int(start.x), fixed32_to_int(start.y),
        fixed32_to_int(end.x), fixed32_to_int(end.y));

    start_node = find_node(tree->root, start);
    end_node = find_node(tree->root, end);

    if (!start_node || !end_node)
        return false;

    // Se a distância é muito pequena, permite o movimento
    move_dist = fixed32_vec_dist(start, end);
    if (move_dist < (FIXED_POINT_SCALE >> 4))
    {
        ft_printf(GREEN"Small movement, allowing\n"DEFAULT);
        return true;
    }

    // Se estão em nós diferentes
    if (start_node != end_node)
    {
        t_fixed_vec32 dir;
        t_fixed32 dist_to_partition;
        
        // Se não há partição, movimento é válido
        if (!start_node->partition)
            return true;

        // Calcula direção do movimento
        dir.x = fixed32_sub(end.x, start.x);
        dir.y = fixed32_sub(end.y, start.y);

        // Calcula distância até a partição
        dist_to_partition = fixed32_abs(fixed32_add(
            fixed32_mul(dir.x, start_node->partition->normal.x),
            fixed32_mul(dir.y, start_node->partition->normal.y)));

        // Se está se movendo praticamente paralelo à partição
        if (dist_to_partition < (FIXED_POINT_SCALE >> 5))
        {
            ft_printf(GREEN"Moving parallel to partition, allowing\n"DEFAULT);
            return true;
        }

        // Verifica se há realmente uma parede entre os pontos
        if (!line_intersect(start, end, start_node->partition))
        {
            ft_printf(GREEN"Different nodes but no wall intersection\n"DEFAULT);
            return true;
        }

        ft_printf(RED"Movement blocked by partition\n"DEFAULT);
        return false;
    }

    // Verifica colisão com paredes no nó atual
    for (i = 0; i < start_node->num_lines; i++)
    {
        t_fixed32 dist = find_nearest_wall(start_node, start);
        
        if (dist != -1)
        {
            ft_printf("Wall distance vs threshold: %d < %d?\n",
                fixed32_to_int(dist), fixed32_to_int(COLLISION_THRESHOLD));

            // Se muito perto da parede, verifica se está se afastando
            if (dist < COLLISION_THRESHOLD)
            {
                t_fixed_vec32 move_dir = vector_sub_fixed32(end, start);
                t_fixed32 dot = fixed32_add(
                    fixed32_mul(move_dir.x, start_node->lines[i]->normal.x),
                    fixed32_mul(move_dir.y, start_node->lines[i]->normal.y));

                // Se está se afastando da parede, permite
                if (dot < 0)
                {
                    ft_printf(GREEN"Close to wall but moving away\n"DEFAULT);
                    continue;
                }
                ft_printf(RED"Too close to wall\n"DEFAULT);
                return false;
            }
        }

        // Verifica interseção com a parede
        if (line_intersect(start, end, start_node->lines[i]))
        {
            ft_printf(RED"Wall intersection detected\n"DEFAULT);
            return false;
        }
    }

    ft_printf(GREEN"Movement valid!\n"DEFAULT);
    return true;
}