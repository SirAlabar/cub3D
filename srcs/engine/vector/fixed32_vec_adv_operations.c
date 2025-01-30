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