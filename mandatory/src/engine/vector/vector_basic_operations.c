/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:31:07 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/04 20:00:44 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double	vector_length(t_vector v)
{
	return (sqrt((v.x * v.x) + (v.y * v.y)));
}

t_vector	vector_add(t_vector v1, t_vector v2)
{
	return (vector_create((v1.x + v2.x), (v1.y + v2.y)));
}

t_vector	vector_sub(t_vector v1, t_vector v2)
{
	return (vector_create((v1.x - v2.x), (v1.y - v2.y)));
}

t_vector	vector_mult(t_vector v, double n)
{
	return (vector_create((v.x * n), (v.y * n)));
}

t_vector	vector_div(t_vector v, double n)
{
	if (n != 0)
		return (vector_create((v.x / n), (v.y / n)));
	return (v);
}
