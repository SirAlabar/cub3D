/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 22:00:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_circle_pixel(t_game *game, t_vector_i pos,
		t_vector_i center, int impact_size)
{
	int	alpha;
	int	color;

	if (vector_i_dist(pos, center) < impact_size / 2)
	{
		alpha = 255 - (game->shot.impact.frame * 51);
		if (alpha < 0)
			alpha = 0;
		color = get_impact_color(game->shot.impact.is_enemy, alpha);
		draw_pixel(game, pos.x, pos.y, color);
	}
}

void	draw_impact_circle(t_game *game, t_vector_i start,
		t_vector_i end, int impact_size)
{
	t_vector_i	pos;
	t_vector_i	center;

	center.x = (start.x + end.x) / 2;
	center.y = (start.y + end.y) / 2;
	pos.y = start.y - 1;
	while (++pos.y < end.y)
	{
		pos.x = start.x - 1;
		while (++pos.x < end.x)
			draw_circle_pixel(game, pos, center, impact_size);
	}
}