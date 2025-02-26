/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_tables_8192.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:09:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/30 12:10:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Gets sine value from lookup table
 * Uses bit shifting for fast index calculation
 * Falls back to regular calculation if tables unavailable
 */
t_fixed32	get_sin_8192(t_fixed_tables_8192 *tables, unsigned int angle)
{
	int	index;

	if (!tables || !tables->is_init)
		return (fixed32_sin(angle));
	index = (angle >> ANGLETOFINESHIFT) & FINEMASK;
	return (tables->sin_table[index]);
}

/*
 * Gets cosine using sine table with phase shift
 * cos(x) = sin(x + π/2)
 * More efficient than separate cosine table
 */
t_fixed32	get_cos_8192(t_fixed_tables_8192 *tables, unsigned int angle)
{
	if (!tables || !tables->is_init)
		return (fixed32_cos(angle));
	return (get_sin_8192(tables, angle + ANG90));
}

/*
 * Gets tangent value from lookup table
 * Direct table lookup for better performance
 * Falls back to calculation if tables unavailable
 */
t_fixed32	get_tan_8192(t_fixed_tables_8192 *tables, unsigned int angle)
{
	int	index;

	if (!tables || !tables->is_init)
		return (fixed32_tan(angle));
	index = (angle >> ANGLETOFINESHIFT) & FINEMASK;
	return (tables->tan_table[index]);
}
