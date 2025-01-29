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

double					vector_heading(t_vector v);
t_vector				vector_rotate(t_vector v, double angle);
t_vector				vector_lerp(t_vector start, t_vector end, double amt);
double					vector_dist(t_vector v1, t_vector v2);
t_vector				vector_limit(t_vector v, double max);