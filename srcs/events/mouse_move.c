/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:20:40 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/12 14:14:33 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	mouse_wrapper(int x, int y, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	return (mouse_move(game, vector_create(x, y)));
}

int	mouse_move(t_game *game, t_vector pos)
{
	t_vector	center;
	double		rotation;

	if (game->last_mouse.x == -1)
	{
		game->last_mouse = pos;
		return (0);
	}
	rotation = (pos.x - game->last_mouse.x) * game->mouse_sensi;
    if (rotation > MAX_ROTATION)
	{
        rotation = MAX_ROTATION;
	}
	rotate_player(game, rotation);
	game->last_mouse = pos;
	center = vector_create(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	if (pos.x < WINDOW_WIDTH / 4 || pos.x > WINDOW_WIDTH * 3 / 4)
	{
		mlx_mouse_move(game->mlx, game->win, center.x, center.y);
		game->last_mouse = center;
	}
	return (0);
}
