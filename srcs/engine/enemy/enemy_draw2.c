/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_draw2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:19:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 11:19:34 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	setup_draw_params(t_draw_params *p, t_enemy *enemy,
	t_sprite_data *s, int stripe)
{
	p->enemy = enemy;
	p->pos.x = stripe;

	if (!enemy->texture)
	{
		p->tex.x = 0;
		return ;
	}
	p->tex.x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
			* enemy->texture->width / s->width) / 256;
}

void	draw_stripe_pixels(t_draw_params *p, t_sprite_data *s)
{
	int	d;

	p->pos.y = s->start_y;
	while (p->pos.y < s->end_y)
	{
		d = (p->pos.y) * 256 - WINDOW_HEIGHT * 128 + s->height * 128;
		p->tex.y = ((d * p->enemy->texture->height) / s->height) / 256;
		if (p->tex.y >= 0 && p->tex.y < p->enemy->texture->height)
			draw_stripe_color(p);
		p->pos.y++;
	}
}

void	draw_enemy_stripe(t_game *game, t_enemy *enemy,
	t_sprite_data *s, int stripe)
{
	t_draw_params	p;

	p.game = game;
	setup_draw_params(&p, enemy, s, stripe);
	if (!enemy || !enemy->texture) {
		return;
	}
	if (p.tex.x >= 0 && p.tex.x < enemy->texture->width)
		draw_stripe_pixels(&p, s);
}
