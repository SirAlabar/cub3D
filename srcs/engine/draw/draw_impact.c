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

static int	get_impact_color(bool is_enemy, int alpha, double distance)
{
	int	shade;
	int	color;

	shade = 20 + (int)(distance * 15);
	if (shade > 80)
		shade = 80;
	if (is_enemy)
		color = (alpha << 24) | (shade << 16) | (shade / 4 << 8) | 0;
	else
		color = (alpha << 24) | (shade << 16) | (shade << 8) | shade;
	return (color);
}

static void	draw_impact_pixel(t_game *game, t_vector_i pos, t_vector_i center,
		int impact_size)
{
	double	distance;
	double	angle;
	int		alpha;
	int		color;

    if (pos.x < 0 || pos.x >= WINDOW_WIDTH || pos.y < 0 || pos.y >= WINDOW_HEIGHT)
	{
        return ;
	}

	distance = vector_i_dist(pos, center);
	if (distance >= impact_size / 6)
		return ;
	angle = atan2(pos.y - center.y, pos.x - center.x) * 4;
	if ((sin(angle + distance) > 0.4 && distance > impact_size / 12)
		|| distance < impact_size / 16)
	{
		alpha = 255 - (game->shot.impact.frame * 51);
		if (alpha < 0)
			alpha = 0;
		color = get_impact_color(game->shot.impact.is_enemy, alpha, distance);
		draw_pixel(game, pos.x, pos.y, color);
	}
}

void	draw_impact_pattern(t_game *game, t_vector_i start, t_vector_i end,
		int impact_size)
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
			draw_impact_pixel(game, pos, center, impact_size);
	}
}
void	draw_impact(t_game *game)
{
	double		elapsed;
	int			impact_size;
	t_vector_i	start;
	t_vector_i	end;

	if (!game->shot.impact.active)
		return ;
	elapsed = get_time_ms() - game->shot.impact.time;
	if (elapsed > 900)
	{
		game->shot.impact.active = false;
		game->shot.active = false;
		return ;
	}
	game->shot.impact.frame = (int)(elapsed / 100) % 5;
	impact_size = abs((int)(WINDOW_HEIGHT / game->shot.ray.perp_wall_dist)) / 6;
	get_impact_bounds(game, &start, &end, impact_size);
	draw_impact_pattern(game, start, end, impact_size);
}
