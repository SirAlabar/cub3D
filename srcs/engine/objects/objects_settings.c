/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/15 20:27:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	transform_sprite(t_game *game, t_sprite *sprite,
				double *transform_x, double *transform_y)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;

	sprite_x = sprite->pos.x - game->p1.pos.x;
	sprite_y = sprite->pos.y - game->p1.pos.y;
	inv_det = 1.0 / (game->p1.plane.x * game->p1.dir.y
			- game->p1.dir.x * game->p1.plane.y);
	*transform_x = inv_det * (game->p1.dir.y * sprite_x
			- game->p1.dir.x * sprite_y);
	*transform_y = inv_det * (-game->p1.plane.y * sprite_x
			+ game->p1.plane.x * sprite_y);
	sprite->distance = *transform_y;
}

static void	set_sprite_limits(int *start, int *end, int sprite_size, int max)
{
	*start = -sprite_size / 2 + max / 2;
	if (*start < 0)
		*start = 0;
	*end = sprite_size / 2 + max / 2;
	if (*end >= max)
		*end = max - 1;
}

static void	calculate_dimensions(t_game *game, t_sprite *sprite,
				double transform_x, double transform_y)
{
	int	sprite_screen_x;
	int	sprite_height;

	sprite_screen_x = (int)((WINDOW_WIDTH / 2)
			* (1 + transform_x / transform_y));
	sprite->tex.x = sprite_screen_x;
	sprite_height = abs((int)(WINDOW_HEIGHT / transform_y));
	sprite->tex.y = sprite_height;
}
