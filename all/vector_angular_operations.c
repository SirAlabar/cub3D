/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angular_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:52:55 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/04 19:54:42 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double	vector_heading(t_vector v)
{
	return (atan2(v.y, v.x));
}

t_vector	vector_rotate(t_vector v, double angle)
{
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	return (vector_create(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y
			* cos_a));
}

t_vector	vector_lerp(t_vector start, t_vector end, double amt)
{
	if (amt <= 0)
		return (start);
	if (amt >= 1)
		return (end);
	return (vector_create(start.x + (end.x - start.x) * amt, start.y + (end.y
				- start.y) * amt));
}

double	vector_dist(t_vector v1, t_vector v2)
{
	return (vector_mag(vector_sub(v1, v2)));
}

t_vector	vector_limit(t_vector v, double max)
{
	double	mag_sq;

	mag_sq = vector_mag_sq(v);
	if (mag_sq > max * max)
		return (vector_mult(vector_normalize(v), max));
	return (v);
}
