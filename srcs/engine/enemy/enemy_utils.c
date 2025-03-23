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

void	set_sprite_dimensions(t_sprite_data *s, double screen_x, int height)
{
	s->height = height;
	s->width = height;
	s->screen_x = screen_x;
	s->start_y = WINDOW_HEIGHT / 2 - height / 2;
	s->end_y = WINDOW_HEIGHT / 2 + height / 2;
	s->start_x = (int)(screen_x - s->width / 2);
	s->end_x = (int)(screen_x + s->width / 2);
}

void	clamp_sprite_boundaries(t_sprite_data *s)
{
	if (s->start_y < 0)
		s->start_y = 0;
	if (s->end_y >= WINDOW_HEIGHT)
		s->end_y = WINDOW_HEIGHT - 1;
	if (s->start_x < 0)
		s->start_x = 0;
	if (s->end_x >= WINDOW_WIDTH)
		s->end_x = WINDOW_WIDTH - 1;
}

void	draw_enemy_sprite(t_game *game, t_enemy *enemy, double screen_x,
	int sprite_height)
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

double	calculate_angle(t_vector to_enemy, t_vector player_dir)
{
	double	dot_product;
	double	mag_to_enemy;
	double	mag_player_dir;
	double	cos_angle;

	dot_product = to_enemy.x * player_dir.x + to_enemy.y * player_dir.y;
	mag_to_enemy = sqrt(to_enemy.x * to_enemy.x + to_enemy.y * to_enemy.y);
	mag_player_dir = sqrt(player_dir.x * player_dir.x + player_dir.y
			* player_dir.y);
	cos_angle = dot_product / (mag_to_enemy * mag_player_dir);
	return (acos(cos_angle));
}

bool should_render_enemy(t_game *game, t_enemy *enemy)
{
    t_vector to_enemy;
    t_vector player_dir;
    double angle_rad;
    double fov_rad;

    // Always render enemies that are very close regardless of view angle
    if (enemy->dist_to_player < 2.5)
        return true;
    
    to_enemy = vector_sub(enemy->pos, game->p1.pos);
    player_dir = game->p1.dir;
    
    // Calculate the angle between player's direction and enemy direction
    angle_rad = calculate_angle(to_enemy, player_dir);
    
    // Expand the FOV by a factor to ensure enemies don't pop in/out at edges
    fov_rad = 2 * atan2(vector_mag(game->p1.plane), 1.0) * 1.5; // Increased from 1.2
    
    // Return true if the enemy is within the player's expanded field of view
    return (angle_rad <= fov_rad / 2);
}
