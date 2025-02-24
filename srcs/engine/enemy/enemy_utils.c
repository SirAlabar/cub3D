/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 09:58:19 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 18:06:18 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	draw_enemy_sprite(t_game *game, t_enemy *enemy, double screen_x, int sprite_height)
{
	t_sprite_data	s;
	int				stripe;

	s.height = sprite_height;
	s.width = sprite_height;
	s.screen_x = screen_x;
	s.start_y = WINDOW_HEIGHT / 2 - sprite_height / 2;
	s.end_y = WINDOW_HEIGHT / 2 + sprite_height / 2;
	s.start_x = (int)(screen_x - s.width / 2);
	s.end_x = (int)(screen_x + s.width / 2);
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

bool	should_render_enemy(t_game *game, t_enemy *enemy)
{
	t_vector to_enemy = vector_sub(enemy->pos, game->p1.pos);
	t_vector player_dir = game->p1.dir;
	double dot_product = to_enemy.x * player_dir.x + to_enemy.y * player_dir.y;
	double magnitude_product = sqrt(to_enemy.x * to_enemy.x + to_enemy.y * to_enemy.y) *
							  sqrt(player_dir.x * player_dir.x + player_dir.y * player_dir.y);

	if (enemy->dist_to_player < 2.0)
		return (true);
	double cos_angle = dot_product / magnitude_product;
	double angle_rad = acos(cos_angle);
	double fov_rad = 2 * atan2(vector_mag(game->p1.plane), 1.0) * 1.2; // 120% of normal FOV
	return	(angle_rad <= fov_rad / 2);
}
