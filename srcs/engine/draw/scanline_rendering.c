/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 20:45:29 by hluiz-ma         ###   ########.fr       */
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

void draw_vertical_line (t_game *g, t_line line, int color)
{
    int y;
    int pixel_pos;

    if (line.start < 0)
        line.start = 0;
    if (line.end >= WINDOW_HEIGHT)
        line.end = WINDOW_HEIGHT - 1;
    y = line.start - 1;
    while (++y <= line.end)
    {
        pixel_pos = (y * g->line_length) + (line.x * (g->bits_per_piel / 8));
        *(unsigned int *)(g->addr + pixel_pos) = color;
    }
}

void set_wall_tex_coords (t_ray *ray, t_game *game, t_wall *wall)
{
    t_texture *tex;

    tex = get_wall_texture(ray, game);
    if (ray->side == 0)
        wall->x = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
    else
        wall->x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
    wall->x -= floor(wall->x);
    wall->tex_x = (int)(wall->x * tex->width);
    if ((ray->side == 0 && ray->dir.x > 0)
    || (ray->side == 1 && ray->dir.y < 0))
    wall->tex_x = tex->width - wall->tex_x - 1;
}

void draw_wall_scanline (t_game *game, t_ray *ray, t_scanline *buffer)
{
    t_wall wall;
    t_texture *tex;
    int y;
    double step;
    double tex_pos;


    wall.height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
    wall.start = -wall.height / 2 + WINDOW_HEIGHT / 2;
    wall.end = wall.height / 2 + WINDOW_HEIGHT / 2;

    if (wall.start < buffer->y_top[x])
        wall.start = buffer->y_top[x];
    if (wall.end > buffer->y_bottom[x])
        wall.end = buffer->y_bottom[x];

    tex = get_wall_texture(ray, game);
    set_wall_tex_coords(ray, game, &wall);
    step = 1.0 * tex->height / wall.height;
    tex_pos = (wall.start - WINDOW_HEIGHT / 2 + wall.height / 2) * step;
    
    y = wall.start - 1;
    while (++y <= wall.end)
    {
        wall.tex_y = (int)tex_pos &(tex->height - 1);
        tex_pos += step;
        wall.color = get_texture_pixel (tex, wall.tex_x, wall.tex_y);
	    wall.color = apply_shade(wall.color, 1.0 / (1.0 + ray->perp_wall_dist * 0.1));
		draw_vertical_line(game, (t_line){x, wall.start, wall.end}, wall.color);      
    }

	buffer->y_top[x] = wall.end + 1;
	buffer->y_bottom[x] = wall.start - 1;    
}
