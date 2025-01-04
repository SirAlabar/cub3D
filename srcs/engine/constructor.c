/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:31:02 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/04 20:00:41 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <cub3d.h>

t_vector    vector_create(double x, double y)
{
    t_vector vec;

    vec.x = x;
    vec.y = y;
    return (vec);
}