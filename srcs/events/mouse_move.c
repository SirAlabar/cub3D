/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:20:40 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:07:06 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	mouse_wrapper(int x, int y, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	return (mouse_move(game, vector_create(x, y)));
}

static void	handle_mouse_rotation(t_game *game, t_vector pos)
{
	double	rotation;

	rotation = (pos.x - game->last_mouse.x) * game->mouse_sensi;
	if (rotation > MAX_ROTATION)
		rotation = MAX_ROTATION;
	if (rotation < -MAX_ROTATION)
		rotation = -MAX_ROTATION;
	move_player(game);
	game->last_mouse = pos;
}

int	mouse_move(t_game *game, t_vector pos)
{
	t_vector	center;
	int			reset_needed;

	if (game->last_mouse.x == -1)
	{
		game->last_mouse = pos;
		return (0);
	}
	center = vector_create(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	reset_needed = (pos.x < WINDOW_WIDTH / 4 || pos.x > WINDOW_WIDTH * 3 / 4);
	if (!reset_needed)
		handle_mouse_rotation(game, pos);
	if (reset_needed)
	{
		mlx_mouse_move(game->mlx, game->win, center.x, center.y);
		game->last_mouse = center;
	}
	return (0);
}
