/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 09:58:19 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/25 10:22:10 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_stripe_color(t_draw_params *p)
{
	int	color;

	color = get_texture_pixel(p->enemy->texture, p->tex.x, p->tex.y);
	if ((color & 0x00FFFFFF) != 0)
		draw_pixel(p->game, p->pos.x, p->pos.y,
			apply_shade(color, 1.0 / (1.0 + p->enemy->dist_to_player * 0.04)));
}

void	draw_enemy_stripe(t_game *game, t_enemy *enemy,
	t_sprite_data *s, int stripe)
{
	t_draw_params	p;
	int				d;

	p.game = game;
	p.enemy = enemy;
	p.pos.x = stripe;
	p.tex.x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
			* enemy->texture->width / s->width) / 256;
	if (p.tex.x >= 0 && p.tex.x < enemy->texture->width)
	{
		p.pos.y = s->start_y;
		while (p.pos.y < s->end_y)
		{
			d = (p.pos.y) * 256 - WINDOW_HEIGHT * 128 + s->height * 128;
			p.tex.y = ((d * enemy->texture->height) / s->height) / 256;
			if (p.tex.y >= 0 && p.tex.y < enemy->texture->height)
				draw_stripe_color(&p);
			p.pos.y++;
		}
	}
}

void	draw_enemy_sprite(t_game *game, t_enemy *enemy,
				double screen_x, int sprite_height)
{
	t_sprite_data	s;
	int				stripe;

	s.height = sprite_height;
	s.width = sprite_height;
	s.screen_x = screen_x;
	s.start_y = (WINDOW_HEIGHT >> 1) - (sprite_height >> 1);
	s.end_y = (WINDOW_HEIGHT >> 1) + (sprite_height >> 1);
	s.start_x = screen_x - (s.width >> 1);
	s.end_x = screen_x + (s.width >> 1);
	if (s.start_y < 0)
		s.start_y = 0;
	if (s.end_y >= WINDOW_HEIGHT)
		s.end_y = WINDOW_HEIGHT - 1;
	if (s.start_x < 0)
		s.start_x = 0;
	if (s.end_x >= WINDOW_WIDTH)
		s.end_x = WINDOW_WIDTH - 1;
	stripe = s.start_x;
	while (stripe < s.end_x)
	{
		draw_enemy_stripe(game, enemy, &s, stripe);
		stripe++;
	}
}

void	calculate_ray_step(t_ray_data *ray, t_vector ray_dir)
{
	if (ray_dir.x < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (ray->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - ray->pos.x) * ray->delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (ray->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - ray->pos.y) * ray->delta_dist.y;
	}
}

t_ray_data	init_ray_data(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;
	t_vector	ray_dir;

	ray_dir = vector_normalize(vector_sub(enemy_pos, game->p1.pos));
	ray.pos = game->p1.pos;
	ray.map_x = (int)ray.pos.x;
	ray.map_y = (int)ray.pos.y;
	ray.delta_dist.x = fabs(1 / ray_dir.x);
	ray.delta_dist.y = fabs(1 / ray_dir.y);
	calculate_ray_step(&ray, ray_dir);
	return (ray);
}
