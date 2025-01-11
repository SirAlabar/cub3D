/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 14:05:12 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	draw_background(t_game *game)
{
	int	x;
	int	y;

	if (!game || !game->img)
	{
		ft_printf("Error: Game or image pointer is NULL\n");
		return (0);
	}
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		x = -1;
		while (++x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				draw_pixel(game, x, y, game->map.ceiling_color);
			else
				draw_pixel(game, x, y, game->map.floor_color);
		}
	}
	return (1);
}

static void	draw_wall_segment(t_game *game, t_ray *ray, double tex_pos, int x)
{
	int			y;
	int			tex_y;
	double		shade;
	t_texture	*tex;
	int			color;

	tex = get_wall_texture(ray, game);
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex_y = (int)tex_pos & (tex->height - 1);
		shade = 1.0 / (1.0 + ray->perp_wall_dist * 0.04);
		color = get_texture_pixel(tex, ray->tex_x, tex_y);
		color = apply_shade(color, shade);
		draw_pixel(game, x, y, color);
		tex_pos += ray->step;
		y++;
	}
}

void	draw_wall(t_game *game, t_ray *ray, int x)
{
	double		wallx;
	double		tex_pos;
	t_texture	*tex;

	tex = get_wall_texture(ray, game);
	if (ray->side == 0)
		wallx = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wallx = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
	wallx -= floor(wallx);
	ray->tex_x = (int)(wallx * tex->width);
	ray->step = 1.0 * tex->height / ray->line_height;
	tex_pos = (ray->draw_start - WINDOW_HEIGHT / 2
			+ ray->line_height / 2) * ray->step;
	draw_wall_segment(game, ray, tex_pos, x);
}
