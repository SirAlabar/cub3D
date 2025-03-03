/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed32_angular_operations.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:08:27 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:09:37 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>


/*
 * Rotates vector by given angle
 * Angle is in binary angle measurement (BAM)
 * Uses sin/cos lookup tables for efficiency
 */
t_fixed_vec32	fixed32_vec_rotate(t_fixed_vec32 v, t_fixed32 angle)
{
	t_fixed_vec32	result;
	t_fixed32		cos_a;
	t_fixed32		sin_a;

	cos_a = fixed32_cos(angle);
	sin_a = fixed32_sin(angle);
	result.x = fixed32_sub(fixed32_mul(v.x, cos_a), fixed32_mul(v.y, sin_a));
	result.y = fixed32_add(fixed32_mul(v.x, sin_a), fixed32_mul(v.y, cos_a));
	return (result);
}

/*
 * Linear interpolation between two vectors
 * amt is between 0 and 1 (fixed point)
 * Returns interpolated vector: start + (end - start) * amt
 */
t_fixed_vec32	fixed32_vec_lerp(t_fixed_vec32 start, t_fixed_vec32 end,
		t_fixed32 amt)
{
	t_fixed_vec32	result;

	if (amt <= 0)
		return (start);
	if (amt >= FIXED_POINT_SCALE)
		return (end);
	result.x = fixed32_add(start.x, fixed32_mul(fixed32_sub(end.x, start.x),
				amt));
	result.y = fixed32_add(start.y, fixed32_mul(fixed32_sub(end.y, start.y),
				amt));
	return (result);
}

/*
 * Calculates distance between two vectors
 * Returns magnitude of difference vector
 * Uses sqrt, so more expensive than distance_sq
 */
t_fixed32	fixed32_vec_dist(t_fixed_vec32 v1, t_fixed_vec32 v2)
{
	t_fixed_vec32	diff;

	diff.x = fixed32_sub(v1.x, v2.x);
	diff.y = fixed32_sub(v1.y, v2.y);
	return (fixed32_vec_mag(diff));
}

/*
 * Limits vector magnitude to maximum value
 * If vector is longer than max, scales it down
 * Preserves direction but limits length
 */
t_fixed_vec32	fixed32_vec_limit(t_fixed_vec32 v, t_fixed32 max)
{
	t_fixed32	mag_sq;
	t_fixed32	max_sq;
	t_fixed32	ratio;

	mag_sq = fixed32_vec_mag_sq(v);
	max_sq = fixed32_mul(max, max);
	if (mag_sq > max_sq)
	{
		ratio = fixed32_div(max, fixed32_sqrt(mag_sq));
		v.x = fixed32_mul(v.x, ratio);
		v.y = fixed32_mul(v.y, ratio);
	}
	return (v);
}

