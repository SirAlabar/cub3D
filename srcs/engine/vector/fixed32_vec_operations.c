/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed32_vec_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:12:09 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:12:35 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
/*
  Calculates dot product of two vectors
  Returns v1.x * v2.x + v1.y * v2.y
  Used for angle calculations and projections
 */
t_fixed32    fixed32_vec_dot(t_fixed_vec32 v1, t_fixed_vec32 v2)
{
    t_fixed32    x_prod;
    t_fixed32    y_prod;

    x_prod = fixed32_mul(v1.x, v2.x);
    y_prod = fixed32_mul(v1.y, v2.y);
    return (fixed32_add(x_prod, y_prod));
}

/*
  Calculates squared magnitude of vector
  Returns x*x + y*y without sqrt
  Useful for comparing distances without sqrt overhead
 */
t_fixed32    fixed32_vec_mag_sq(t_fixed_vec32 v)
{
    return (fixed32_vec_dot(v, v));
}

/*
  Calculates actual magnitude (length) of vector
  Returns sqrt(x*x + y*y)
  Uses fixed point square root
 */
t_fixed32    fixed32_vec_mag(t_fixed_vec32 v)
{
    return (fixed32_sqrt(fixed32_vec_mag_sq(v)));
}

/*
  Normalizes vector (makes length = 1)
  Returns vector divided by its magnitude
  Preserves direction but standardizes length
 */
t_fixed_vec32    fixed32_vec_normalize(t_fixed_vec32 v)
{
    t_fixed32        mag;
    t_fixed_vec32    result;

    mag = fixed32_vec_mag(v);
    result.x = 0;
    result.y = 0;
    if (mag != 0)
    {
        result.x = fixed32_div(v.x, mag);
        result.y = fixed32_div(v.y, mag);
    }
    return (result);
}
