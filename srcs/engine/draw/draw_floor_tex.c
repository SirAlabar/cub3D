/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_tex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:00:00 by seu-nome          #+#    #+#             */
/*   Updated: 2025/03/24 19:46:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	init_floor_rays(t_game *game, t_floor_data *f)
{
	f->ray_dir0 = vector_create(game->p1.dir.x - game->p1.plane.x,
			game->p1.dir.y - game->p1.plane.y);
	f->ray_dir1 = vector_create(game->p1.dir.x + game->p1.plane.x,
			game->p1.dir.y + game->p1.plane.y);
}

static void	calculate_floor_step(t_floor_data *f, double row)
{
	f->floor_step = vector_create(row * (f->ray_dir1.x - f->ray_dir0.x)
			/ WINDOW_WIDTH, row * (f->ray_dir1.y - f->ray_dir0.y)
			/ WINDOW_WIDTH);
}

static void	calculate_floor_pos(t_game *game, t_floor_data *f, double row)
{
	f->floor_pos = vector_create(game->p1.pos.x + row * f->ray_dir0.x,
			game->p1.pos.y + row * f->ray_dir0.y);
}

static void	draw_floor_line(t_game *game, t_floor_data *f, int y, double row)
{
	int			x;
	t_vector_i	tex_pos;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		tex_pos.x = (int)(game->floor.width * (f->floor_pos.x
					- floor((int)f->floor_pos.x))) % game->floor.width;
		tex_pos.y = (int)(game->floor.height * (f->floor_pos.y
					- floor((int)f->floor_pos.y))) % game->floor.height;
		draw_pixel(game, x, y, apply_shade(get_texture_pixel(&game->floor,
					tex_pos.x, tex_pos.y), 1.0 / (1.0 + row * 0.05)));
		f->floor_pos.x += f->floor_step.x;
		f->floor_pos.y += f->floor_step.y;
		x++;
	}
}

void	draw_floor(t_game *game)
{
	int				y;
	double			row_distance;
	double			pos_z;
	t_floor_data	f;

	if (!game->floor.img)
		return ;
	pos_z = 0.5 * WINDOW_HEIGHT;
	y = (WINDOW_HEIGHT >> 1);
	while (y < WINDOW_HEIGHT)
	{
		init_floor_rays(game, &f);
		row_distance = pos_z / (y - (WINDOW_HEIGHT >> 1));
		calculate_floor_step(&f, row_distance);
		calculate_floor_pos(game, &f, row_distance);
		draw_floor_line(game, &f, y, row_distance);
		y++;
	}
}
