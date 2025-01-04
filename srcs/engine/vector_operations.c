/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:31:05 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/04 20:00:38 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <cub3d.h>

double vector_dot(t_vector v1, t_vector v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

double vector_mag_sq(t_vector v)
{
    return (v.x * v.x + v.y * v.y);
}

double vector_mag(t_vector v)
{
    return (sqrt(vector_mag_sq(v)));
}

t_vector vector_normalize(t_vector v)
{
    double mag;

    mag = vector_mag(v);
    if (mag != 0)
        return (vector_div(v, mag));
    return (v);
}
