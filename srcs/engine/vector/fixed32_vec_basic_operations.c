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

t_fixed_vec32 vector_add_fixed32(t_fixed_vec32 v1, t_fixed_vec32 v2)
{
    return ((t_fixed_vec32){
        fixed32_add(v1.x, v2.x),
        fixed32_add(v1.y, v2.y)
    });
}

t_fixed_vec32 vector_sub_fixed32(t_fixed_vec32 v1, t_fixed_vec32 v2)
{
    return ((t_fixed_vec32){
        fixed32_sub(v1.x, v2.x),
        fixed32_sub(v1.y, v2.y)
    });
}

t_fixed_vec32 vector_mult_fixed32(t_fixed_vec32 v, t_fixed32 n)
{
    return ((t_fixed_vec32){
        fixed32_mul(v.x, n),
        fixed32_mul(v.y, n)
    });
}

t_fixed_vec32 vector_div_fixed32(t_fixed_vec32 v, t_fixed32 n)
{
    if (n != 0)
        return ((t_fixed_vec32){
            fixed32_div(v.x, n),
            fixed32_div(v.y, n)
        });
    return (v);
}
