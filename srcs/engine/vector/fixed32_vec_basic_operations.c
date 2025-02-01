/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed32_vec_basic_operations.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:06:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:08:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
  Basic addition of two fixed point numbers
  Simply adds the raw values since fixed point maintains position
  Returns INT32_MAX/MIN on overflow
 */
t_fixed32	fixed32_add(t_fixed32 a, t_fixed32 b)
{
	if (b > 0 && a > INT32_MAX - b)
		return (INT32_MAX);
	if (b < 0 && a < INT32_MIN - b)
		return (INT32_MIN);
	return (a + b);
}

/*
  Basic subtraction of two fixed point numbers
  Simply subtracts the raw values since fixed point maintains position
  Returns INT32_MAX/MIN on overflow
 */
t_fixed32	fixed32_sub(t_fixed32 a, t_fixed32 b)
{
	if (b < 0 && a > INT32_MAX + b)
		return (INT32_MAX);
	if (b > 0 && a < INT32_MIN + b)
		return (INT32_MIN);
	return (a - b);
}

/*
  Multiplication of two fixed point numbers
  Uses int64_t for intermediate calculation to prevent overflow
  Shifts right by FIXED32_BITS to align decimal point
 */
t_fixed32	fixed32_mul(t_fixed32 a, t_fixed32 b)
{
	int64_t	result;

	result = ((int64_t)a * (int64_t)b) >> FIXED_POINT_BITS;
	if (result > INT32_MAX)
		return (INT32_MAX);
	if (result < INT32_MIN)
		return (INT32_MIN);
	return ((t_fixed32)result);
}

/*
  Division of two fixed point numbers
  Uses int64_t and shifts left first to preserve precision
  Handles division by zero and overflow
 */
t_fixed32	fixed32_div(t_fixed32 a, t_fixed32 b)
{
	int64_t	temp;

	if (b == 0)
	{
		if (a >= 0)
			return (INT32_MAX);
		return (INT32_MIN);
	}
	temp = ((int64_t)a << FIXED_POINT_BITS) / b;
	if (temp > INT32_MAX)
		return (INT32_MAX);
	if (temp < INT32_MIN)
		return (INT32_MIN);
	return ((t_fixed32)temp);
}
