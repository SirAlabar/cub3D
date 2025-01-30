/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_POINT_H
# define FIXED_POINT_H

# include <stdint.h>

/*
Fixed point configuration
*/
# define FIXED_POINT_BITS 16
# define FIXED_POINT_SCALE 65536 /*(1 << FIXED_POINT_BITS)*/
# define FIXED_POINT_MASK 65535 /*(FIXED_POINT_SCALE - 1)*/   

/*
Common constants in fixed point representation
*/
# define FIXED32_HALF 32768    /* (FIXED32_SCALE >> 1) */
# define FIXED32_PI 205887     /* π in fixed point (≈3.14159 * 2^16) */
# define FIXED32_2PI 411775    /* 2π in fixed point */
# define FIXED32_PI_2 102944   /* π/2 in fixed point */
# define FIXED32_E 178145      /* e in fixed point */

/*
Types
*/
typedef int32_t t_fixed32;
typedef int64_t t_fixed64;

typedef struct s_fixed_vec32
{
    fixed32_t x;
    fixed32_t y;
}   t_fidex_vec32;

typedef struct s_fixed_vec64
{
    fixed64_t x;
    fixed64_t y;
}   t_fixed_vec64;


// Basic conversion functions
t_fixed32 float_to_fixed(float value);
t_fixed32 int_to_fixed(int value);
float fixed_to_float(t_fixed32 value);
int fixed_to_int(t_fixed32 value);

// Basic arithmetic
t_fixed32 fixed32_add(t_fixed32 a, t_fixed32 b);
t_fixed32 fixed32_sub(t_fixed32 a, t_fixed32 b);
t_fixed32 fixed32_mul(t_fixed32 a, t_fixed32 b);
t_fixed32 fixed32_div(t_fixed32 a, t_fixed32 b);

// Advanced operations
t_fixed32 fixed32_sqrt(t_fixed32 value);
t_fixed32 fixed32_sin(t_fixed32 angle);
t_fixed32 fixed32_cos(t_fixed32 angle);
t_fixed32 fixed32_tan(t_fixed32 angle);


#endif