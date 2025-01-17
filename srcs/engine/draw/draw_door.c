/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 20:13:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static int	get_line_height(double perp_wall_dist)
{
	return ((int)(WINDOW_HEIGHT / perp_wall_dist));
}

static void	get_draw_limits(int line_height, int *start, int *end)
{
	*start = -line_height / 2 + WINDOW_HEIGHT / 2;
	if (*start < 0)
		*start = 0;
	*end = line_height / 2 + WINDOW_HEIGHT / 2;
	if (*end >= WINDOW_HEIGHT)
		*end = WINDOW_HEIGHT - 1;
}

static double	get_wall_x(t_game *game, t_ray *ray)
{
	t_vector	wall_pos;
	t_vector	ray_vector;

	ray_vector = vector_mult(ray->dir, ray->perp_wall_dist);
	wall_pos = vector_add(game->p1.pos, ray_vector);
	if (ray->side == 0)
		return (wall_pos.y - floor(wall_pos.y));
	return (wall_pos.x - floor(wall_pos.x));
}

static void	draw_vertical_line(t_game *game, t_door *door, int x, t_ray *ray)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	t_vector_i	tex;
	double		step;
	double		tex_pos;

	line_height = get_line_height(ray->perp_wall_dist);
	get_draw_limits(line_height, &draw_start, &draw_end);
	tex.x = (int)(get_wall_x(game, ray) * game->door_system->door_texture.width);
	tex.x += (int)(door->animation * game->door_system->door_texture.width);
	step = 1.0 * game->door_system->door_texture.height / line_height;
	tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	while (draw_start < draw_end)
	{
		tex.y = (int)tex_pos & (game->door_system->door_texture.height - 1);
		tex_pos += step;
		draw_pixel(game, x, draw_start, apply_shade(get_texture_pixel(
			&game->door_system->door_texture, tex.x, tex.y),
			1.0 / (1.0 + ray->perp_wall_dist * 0.1)));
		draw_start++;
	}
}

void	render_door(t_game *game, t_ray *ray, int x)
{
	t_door	*door;

	door = find_door(game, ray->map_x, ray->map_y);
	if (!door)
		return ;
	if ((door->orient == DOOR_VERTICAL && ray->side == 1)
		|| (door->orient == DOOR_HORIZONTAL && ray->side == 0))
		return ;
	draw_vertical_line(game, door, x, ray);
}