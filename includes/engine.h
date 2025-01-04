/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:28:20 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/04 19:05:07 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <cub3d.h>
# include <math.h>

typedef struct s_vector
{
	double	x;
	double	y;
}			t_vector;

// Constructor
t_vector	vector_create(double x, double y);

// Basic operations
t_vector	vector_add(t_vector v1, t_vector v2);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vector_mult(t_vector v, double n);
t_vector	vector_div(t_vector v, double n);

// Vector operations
double		vector_dot(t_vector v1, t_vector v2);
double		vector_mag(t_vector v);
double		vector_mag_sq(t_vector v);
t_vector	vector_normalize(t_vector v);

// Angular operations
double		vector_heading(t_vector v);
t_vector	vector_rotate(t_vector v, double angle);
t_vector	vector_lerp(t_vector start, t_vector end, double amt);
double		vector_dist(t_vector v1, t_vector v2);
t_vector	vector_limit(t_vector v, double max);

#endif