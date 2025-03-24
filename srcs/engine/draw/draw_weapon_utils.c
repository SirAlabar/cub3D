/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_weapon_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:47:11 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:47:11 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_vector_i	get_weapon_dimensions(t_texture *gun)
{
	t_vector_i	dim;

	dim.x = WINDOW_WIDTH >> 2;
	dim.y = (dim.x * gun->height) / gun->width;
	return (dim);
}

t_vector_i	get_weapon_position(t_vector_i dim)
{
	t_vector_i	pos;

	pos.x = (WINDOW_WIDTH - dim.x) >> 1;
	pos.y = WINDOW_HEIGHT - dim.y;
	return (pos);
}

t_vector	get_scale_factors(t_texture *gun, t_vector_i dim)
{
	t_vector	scale;

	scale.x = (double)gun->width / dim.x;
	scale.y = (double)gun->height / dim.y;
	return (scale);
}
