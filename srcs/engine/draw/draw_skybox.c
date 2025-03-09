/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seu-nome <seu-email@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:00:00 by seu-nome          #+#    #+#             */
/*   Updated: 2025/03/03 10:00:00 by seu-nome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	draw_skybox_line(t_game *game, int y, double player_angle)
{
	int		x;
	double	angle;
	int		tex_x;
	int		tex_y;
	double	fov_radians;

	fov_radians = FOV * M_PI / 180.0;
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		angle = player_angle + (((double)(x - (WINDOW_WIDTH >> 1))
					*fov_radians) / WINDOW_WIDTH);
		while (angle < 0)
			angle += 2 * M_PI;
		while (angle >= 2 * M_PI)
			angle -= 2 * M_PI;
		tex_x = (int)((angle / (2 * M_PI)) * game->skybox.width)
			% game->skybox.width;
		tex_y = (y * game->skybox.height) / (WINDOW_HEIGHT >> 1);
		draw_pixel(game, x, y, get_texture_pixel(&game->skybox, tex_x, tex_y));
		x++;
	}
}

void	draw_skybox(t_game *game)
{
	int		y;
	double	player_angle;

	if (!game->skybox.img)
		return ;
	player_angle = atan2(game->p1.dir.y, game->p1.dir.x);
	y = 0;
	while (y < (WINDOW_HEIGHT >> 1))
	{
		draw_skybox_line(game, y, player_angle);
		y++;
	}
}
