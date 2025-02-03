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

/*
** Check if point-line distance is within threshold
** Returns true if point is too close to line
*/
static bool	check_point_line_distance(t_fixed_vec32 point, t_bsp_line *line)
{
	t_fixed_vec32	line_dir;
	t_fixed_vec32	to_point;
	t_fixed32		cross_product;
	t_fixed32		line_length;

	line_dir.x = fixed32_sub(line->end.x, line->start.x);
	line_dir.y = fixed32_sub(line->end.y, line->start.y);
	to_point.x = fixed32_sub(point.x, line->start.x);
	to_point.y = fixed32_sub(point.y, line->start.y);
	cross_product = fixed32_sub(fixed32_mul(line_dir.x, to_point.y),
			fixed32_mul(line_dir.y, to_point.x));
	line_length = fixed32_sqrt(fixed32_add(fixed32_mul(line_dir.x, line_dir.x),
				fixed32_mul(line_dir.y, line_dir.y)));
	return (fixed32_abs(fixed32_div(cross_product, line_length))
		< COLLISION_THRESHOLD);
}

/*
** Check for collision with lines in node
** Returns true if collision detected
*/
static bool	check_node_collision(t_bsp_node *node, t_fixed_vec32 point)
{
	int	i;

	if (!node)
		return (false);
	i = 0;
	while (i < node->num_lines)
	{
		if (check_point_line_distance(point, node->lines[i]))
			return (true);
		i++;
	}
	return (false);
}

/*
** Find closest wall to a point
** Returns distance to nearest wall or -1 if none found
*/
t_fixed32	find_nearest_wall(t_bsp_node *node, t_fixed_vec32 point)
{
	t_fixed32	min_dist;
	t_fixed32	curr_dist;
	int			i;

	if (!node)
		return (-1);
	min_dist = INT32_MAX;
	i = 0;
	while (i < node->num_lines)
	{
		curr_dist = fixed32_abs(fixed32_mul(node->lines[i]->normal.x,
				fixed32_sub(point.x, node->lines[i]->start.x))
			+ fixed32_mul(node->lines[i]->normal.y,
				fixed32_sub(point.y, node->lines[i]->start.y)));
		if (curr_dist < min_dist)
			min_dist = curr_dist;
		i++;
	}
	return (min_dist);
}

/*
** Check if movement from start to end intersects any walls
** Returns true if path is clear
*/
bool	check_movement_valid(t_bsp_tree *tree, t_fixed_vec32 start,
		t_fixed_vec32 end)
{
	t_bsp_node		*curr_node;
	t_fixed_vec32	step;
	t_fixed_vec32	curr_pos;
	int				num_steps;
	int				i;

	curr_node = find_node(tree->root, start);
	if (!curr_node)
		return (false);
	step.x = fixed32_div(fixed32_sub(end.x, start.x), int_to_fixed32(10));
	step.y = fixed32_div(fixed32_sub(end.y, start.y), int_to_fixed32(10));
	num_steps = 10;
	curr_pos = start;
	i = 0;
	while (i < num_steps)
	{
		curr_pos.x = fixed32_add(curr_pos.x, step.x);
		curr_pos.y = fixed32_add(curr_pos.y, step.y);
		if (check_node_collision(curr_node, curr_pos))
			return (false);
		curr_node = find_node(tree->root, curr_pos);
		if (!curr_node)
			return (false);
		i++;
	}
	return (true);
}

/*
** Adjust movement vector to slide along walls
** Modifies movement to prevent wall penetration
*/
void	adjust_collision_response(t_bsp_node *node, t_fixed_vec32 *movement)
{
	t_fixed_vec32	normal;
	t_fixed32		dot;
	int				i;

	if (!node || !movement)
		return ;
	i = 0;
	while (i < node->num_lines)
	{
		normal = node->lines[i]->normal;
		dot = fixed32_add(fixed32_mul(movement->x, normal.x),
				fixed32_mul(movement->y, normal.y));
		if (dot < 0)
		{
			movement->x = fixed32_sub(movement->x,
					fixed32_mul(normal.x, dot));
			movement->y = fixed32_sub(movement->y,
					fixed32_mul(normal.y, dot));
		}
		i++;
	}
}
