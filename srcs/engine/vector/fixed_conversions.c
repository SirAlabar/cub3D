/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_conversions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 22:06:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 22:08:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
  Converts float to fixed point
  Multiplies by 2^16 (left shift 16)
  Handles overflow
 */
t_fixed32	float_to_fixed32(float value)
{
	float	temp;

	temp = value * FIXED_POINT_SCALE;
	if (temp > INT32_MAX)
		return (INT32_MAX);
	if (temp < INT32_MIN)
		return (INT32_MIN);
	return ((t_fixed32)temp);
}

/*
  Converts integer to fixed point
  Multiplies by 2^16 (left shift 16)
  Handles overflow
 */
t_fixed32	int_to_fixed32(int value)
{
	int64_t	temp;

	temp = (int64_t)value * FIXED_POINT_SCALE;
	if (temp > INT32_MAX)
		return (INT32_MAX);
	if (temp < INT32_MIN)
		return (INT32_MIN);
	return ((t_fixed32)temp);
}

/*
  Converts fixed point to float
  Divides by 2^16
 */
float	fixed32_to_float(t_fixed32 value)
{
	return ((float)value / FIXED_POINT_SCALE);
}

/*
  Converts fixed point to integer
  Right shifts by 16 (divides by 2^16)
 */
int	fixed32_to_int(t_fixed32 value)
{
	return ((int)(value >> FIXED_POINT_BITS));
}
