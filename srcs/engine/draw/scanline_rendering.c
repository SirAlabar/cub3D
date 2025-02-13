/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/13 19:20:04 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_scanline_buffer(t_scanline *buffer)
{
	int	x;

	x = -1;
	while (++x < WINDOW_WIDTH)
	{
		buffer->y_ceil[x] = 0;
		buffer->y_floor[x] = WINDOW_HEIGHT - 1;
		buffer->y_top[x] = 0;
		buffer->y_bottom[x] = WINDOW_HEIGHT - 1;
	}
}

static void	put_pixel(t_game *game, int x, int y, unsigned int color)
{
	int	pixel_pos;

	if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
		return ;
	pixel_pos = (y * game->line_length) + (x * (game->bits_per_pixel / 8));
	*(unsigned int *)(game->addr[game->current_buffer] + pixel_pos) = color;
}

void	draw_scanline(t_game *game, t_fixed_vec32 start, t_fixed_vec32 end,
		t_scanline *buffer)
{
	t_fixed32	dx;
	t_fixed32	dy;
	t_fixed32	step;
	t_fixed32	x;
	t_fixed32	height;

	dx = fixed32_sub(end.x, start.x);
	dy = fixed32_sub(end.y, start.y);
	
	if (fixed32_abs(dx) > fixed32_abs(dy))
		step = fixed32_abs(dx);
	else
		step = fixed32_abs(dy);

	dx = fixed32_div(dx, step);
	dy = fixed32_div(dy, step);

	x = start.x;
	height = start.y;

	while (step > 0)
	{
		if (fixed32_to_int(x) >= 0 && fixed32_to_int(x) < WINDOW_WIDTH)
		{
			t_fixed32 wall_height = fixed32_div(int_to_fixed32(WINDOW_HEIGHT), 
				height);
			t_fixed32 screen_y = fixed32_div(
				fixed32_sub(int_to_fixed32(WINDOW_HEIGHT / 2), wall_height), 
				int_to_fixed32(2));

			if (screen_y < buffer->y_top[fixed32_to_int(x)])
			{
				put_pixel(game, fixed32_to_int(x), fixed32_to_int(screen_y), 
					0xFFFFFF);
				buffer->y_top[fixed32_to_int(x)] = fixed32_to_int(screen_y);
			}
		}
		x = fixed32_add(x, dx);
		height = fixed32_add(height, dy);
		step = fixed32_sub(step, FIXED_POINT_SCALE);
	}
}

void	draw_line_segment(t_game *game, t_bsp_line *line, t_scanline *buffer)
{
	t_fixed_vec32	view_start;
	t_fixed_vec32	view_end;

	view_start = vector_sub_fixed32(line->start, game->p1.pos);
	view_end = vector_sub_fixed32(line->end, game->p1.pos);

	view_start = fixed32_vec_rotate(view_start, game->p1.angle);
	view_end = fixed32_vec_rotate(view_end, game->p1.angle);

	if (view_start.y <= 0 && view_end.y <= 0)
		return ;

	draw_scanline(game, view_start, view_end, buffer);
}