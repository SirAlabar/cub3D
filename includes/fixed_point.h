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
# define FIXED_POINT_SCALE (1 << FIXED_POINT_BITS)
# define FIXED_POINT_MASK (FIXED_POINT_SCALE - 1)

/*
Types
*/

typedef int32_t fixed32_t;
typedef int64_t fixed62_t;

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
fixed_t float_to_fixed(float value);
fixed_t int_to_fixed(int value);
float fixed_to_float(fixed_t value);
int fixed_to_int(fixed_t value);

// Basic arithmetic
fixed_t fixed_add(fixed_t a, fixed_t b);
fixed_t fixed_sub(fixed_t a, fixed_t b);
fixed_t fixed_mul(fixed_t a, fixed_t b);
fixed_t fixed_div(fixed_t a, fixed_t b);

// Advanced operations
fixed_t fixed_sqrt(fixed_t value);
fixed_t fixed_sin(fixed_t angle);
fixed_t fixed_cos(fixed_t angle);
fixed_t fixed_tan(fixed_t angle);