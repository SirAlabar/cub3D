/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_int_operations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:16:06 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/12 19:48:27 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_vector_i	vector_i_create(int x, int y)
{
	t_vector_i	v;

	v.x = x;
	v.y = y;
	return (v);
}

bool	vector_i_equals(t_vector_i a, t_vector_i b)
{
	return (a.x == b.x && a.y == b.y);
}

t_vector_i	vector_to_vector_i(t_vector v)
{
	t_vector_i	vi;

	vi.x = (int)v.x;
	vi.y = (int)v.y;
	return (vi);
}

t_vector	vector_i_to_vector(t_vector_i v)
{
	return (vector_create(v.x, v.y));
}
/*
double	vector_i_dist(t_vector_i v1, t_vector_i v2)
{
	return (sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2)));
}*/
double	vector_i_dist(t_vector_i v1, t_vector_i v2)
{
	double	dx;
	double	dy;
	
	dx = (double)(v1.x - v2.x);
	dy = (double)(v1.y - v2.y) * 1.5;
	return (sqrt(dx * dx + dy * dy));
}