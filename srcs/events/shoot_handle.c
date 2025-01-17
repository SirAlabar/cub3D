/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:29 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:55:31 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void set_impact_position(t_game *game)
{
    game->shot.impact.x = game->p1.pos.x + game->shot.ray.perp_wall_dist
        * game->shot.ray.dir.x;
    game->shot.impact.y = game->p1.pos.y + game->shot.ray.perp_wall_dist
        * game->shot.ray.dir.y;
}

void fire_shot (t_game *game)
{
    if (game->shot.active)
        return ;
    game->shot.active = true;
    game->shot.impact.is_enemy = false;
    init_ray(&game->shot.ray, game);
    perform_dda(&game->shot.ray, game);
    if (game->shot.ray.hit)
    {
        game->shot.impact.active = true;
        game->shot.impact.time = get_time_ms();
        game->shot.impact.frame = 0;
        set_impact_position(game);
    }
}

static void	get_impact_bounds(t_game *game, t_vector_i *start,
		t_vector_i *end, int impact_size)
{
	t_vector	sprite_pos;
	t_vector	transform;
	double		inv_det;
	int			sprite_screen_x;

	sprite_pos = vector_sub(
		vector_create(game->shot.impact.x, game->shot.impact.y),
		game->p1.pos);
	inv_det = 1.0 / vector_dot(game->p1.plane,
		vector_create(-game->p1.dir.y, game->p1.dir.x));
	transform.x = inv_det * vector_dot(game->p1.dir,
		vector_create(sprite_pos.y, -sprite_pos.x));
	transform.y = inv_det * vector_dot(game->p1.plane,
		vector_create(-sprite_pos.x, -sprite_pos.y));
	sprite_screen_x = (int)((WINDOW_WIDTH / 2)
			* (1 + transform.x / transform.y));
	start->x = sprite_screen_x - impact_size / 2;
	end->x = sprite_screen_x + impact_size / 2;
	start->y = WINDOW_HEIGHT / 2 - impact_size / 2;
	end->y = WINDOW_HEIGHT / 2 + impact_size / 2;
}

static void	limit_bounds(t_vector_i *start, t_vector_i *end)
{
	if (start->x < 0)
		start->x = 0;
	if (end->x >= WINDOW_WIDTH)
		end->x = WINDOW_WIDTH - 1;
	if (start->y < 0)
		start->y = 0;
	if (end->y >= WINDOW_HEIGHT)
		end->y = WINDOW_HEIGHT - 1;
}

static int	get_impact_color(bool is_enemy, int alpha)
{
	int	color;

	if (is_enemy)
		color = (alpha << 24) | (0xFF << 16);
	else
		color = (alpha << 24) | (0xFF << 16) | (0xFF << 8);
	return (color);
}


static void	draw_circle_pixel(t_game *game, t_vector_i pos,
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

static void	draw_impact_circle(t_game *game, t_vector_i start,
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
