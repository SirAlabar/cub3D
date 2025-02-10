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

t_fixed32	fix_min(t_fixed32 a, t_fixed32 b)
{
	if (a < b)
		return (a);
	return (b);
}

t_fixed32	fix_max(t_fixed32 a, t_fixed32 b)
{
	if (a > b)
		return (a);
	return (b);
}
