/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:35:41 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/25 10:48:54 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEALTH_H
# define HEALTH_H

typedef struct s_bar
{
	int	start_x;
	int	start_y;
	int	width;
	int	height;
	int	color;
}	t_bar;

void	draw_health_bar(t_game *game);

#endif
