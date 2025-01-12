/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/12 13:10:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void init_scanline_buffer(t_scanline *buffer)
{
    int x;
    
    x = -1;

    while(++x < WINDOW_WIDTH)
    {
        buffer->y_top[x] = 0;
        buffer->y_bottom[x] = WINDOW_HEIGHT - 1;
    }
}

void	init_wall_drawing(t_wall *wall)
{
	wall->height = (int)(WINDOW_HEIGHT / wall->ray->perp_wall_dist);
	wall->start = -wall->height / 2 + WINDOW_HEIGHT / 2;
	wall->end = wall->height / 2 + WINDOW_HEIGHT / 2;
	if (wall->start < wall->buffer->y_top[wall->x])
		wall->start = wall->buffer->y_top[wall->x];
	if (wall->end > wall->buffer->y_bottom[wall->x])
		wall->end = wall->buffer->y_bottom[wall->x];
	wall->texture = get_wall_texture(wall->ray, wall->game);
	set_wall_tex_coords(wall);
	wall->step = 1.0 * wall->texture->height / wall->height;
	wall->tex_pos = (wall->start - WINDOW_HEIGHT / 2
			+ wall->height / 2) * wall->step;
}

void	set_wall_tex_coords(t_wall *wall)
{
	if (wall->ray->side == 0)
		wall->pos.x = wall->game->p1.pos.y + wall->ray->perp_wall_dist * wall->ray->dir.y;
	else
		wall->pos.x = wall->game->p1.pos.x + wall->ray->perp_wall_dist * wall->ray->dir.x;
	wall->pos.x -= floor(wall->pos.x);
	wall->tex.x = (int)(wall->pos.x * wall->texture->width);
	if ((wall->ray->side == 0 && wall->ray->dir.x > 0)
		|| (wall->ray->side == 1 && wall->ray->dir.y < 0))
		wall->tex.x = wall->texture->width - wall->tex.x - 1;
}

static void	put_wall_pixel(t_wall *wall, t_vector_i pos)
{
	int	pixel_pos;

	wall->tex.y = (int)wall->tex_pos & (wall->texture->height - 1);
	wall->color = get_texture_pixel(wall->texture,
			wall->tex.x, wall->tex.y);
	wall->color = apply_shade(wall->color,
			1.0 / (1.0 + wall->ray->perp_wall_dist * 0.1));
	pixel_pos = (pos.y * wall->game->line_length) + 
		(pos.x * (wall->game->bits_per_pixel / 8));
	*(unsigned int *)(wall->game->addr + pixel_pos) = wall->color;
	wall->tex_pos += wall->step;
}

void	draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
{
	t_wall		wall;
	t_vector_i	pos;

	wall.game = game;
	wall.ray = ray;
	wall.x = x;
	wall.buffer = buffer;
	init_wall_drawing(&wall);
	pos.x = x;
	pos.y = wall.start - 1;
	while (++pos.y <= wall.end)
		put_wall_pixel(&wall, pos);
	buffer->y_top[x] = wall.end + 1;
	buffer->y_bottom[x] = wall.start - 1;
}
