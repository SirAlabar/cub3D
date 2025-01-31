/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed32_vec_adv_operations.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:07:45 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:09:05 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
  Newton-Raphson method square root for fixed point
  Iteratively improves guess until sufficient precision
  Returns fixed point square root of input
 */
t_fixed32    fixed32_sqrt(t_fixed32 value)
{
    t_fixed32   guess;
    t_fixed32   new_guess;
    size_t      i;

    if (value <= 0)
        return (0);
    guess = value >> 1;
    if (guess == 0)
        guess = 1;
    i = 8;
    while (i > 0)
    {
        new_guess = fixed32_div(fixed32_add(fixed32_div(value, guess), guess),
                                int_to_fixed32(2));
        if (abs(new_guess - guess) <= 1)
            break;
        guess = new_guess;
        i--;                            
    }
    return (guess);        
}

/*
  Sine function using lookup table
  Angle is in binary angle measurement (BAM)
  Returns fixed point sine value between -1 and 1
 */
t_fixed32    fixed32_sin(t_fixed32 angle)
{
    double    float_angle;
    double    sin_val;

    float_angle = (angle * 2.0 * M_PI) / (1 << FIXED32_BITS);
    sin_val = sin(float_angle);
    return (float_to_fixed32(sin_val));
}

/*
  Cosine function using lookup table
  Angle is in binary angle measurement (BAM)
  Returns fixed point cosine value between -1 and 1
 */
t_fixed32    fixed32_cos(t_fixed32 angle)
{
    return (fixed32_sin(angle + (FIXED32_PI >> 1)));
}

/*
  Tangent function using sine and cosine
  Angle is in binary angle measurement (BAM)
  Returns fixed point tangent value
 */
t_fixed32    fixed32_tan(t_fixed32 angle)
{
    t_fixed32    cos_val;

    cos_val = fixed32_cos(angle);
    if (cos_val == 0)
    {
        if (angle > 0)
            return (INT32_MAX);
        return (INT32_MIN);
    }
    return (fixed32_div(fixed32_sin(angle), cos_val));
}
