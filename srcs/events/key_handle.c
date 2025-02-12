/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 15:10:49 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Converts keyboard state into movement commands
** Handles movement, turning, and action inputs
*/
void	build_player_cmd(t_player *player)
{
	t_keys	*keys;
	t_cmd	*cmd;

	keys = &player->keys;
	cmd = &player->cmd;
	cmd->forward = 0;
	cmd->side = 0;
	cmd->turn = 0;
	cmd->attack = 0;
	cmd->use = 0;
	cmd->jump = 0;
	if (keys->w)
		cmd->forward += FIXED_POINT_SCALE;
	if (keys->s)
		cmd->forward -= FIXED_POINT_SCALE;
	if (keys->d)
		cmd->side += FIXED_POINT_SCALE;
	if (keys->a)
		cmd->side -= FIXED_POINT_SCALE;
	if (keys->left)
		cmd->turn -= ANG90 / 32;
	if (keys->right)
		cmd->turn += ANG90 / 32;
}

/*
** Handles key press events, updating player input state
** Returns 0 to keep MLX event loop running
*/
int	handle_key_press(int keycode, t_game *game)
{
	t_player	*p;

	p = &game->p1;
	if (keycode == KEY_ESC)
		return (close_window(game));
	if (keycode == KEY_W)
		p->keys.w = 1;
	if (keycode == KEY_S)
		p->keys.s = 1;
	if (keycode == KEY_D)
		p->keys.d = 1;
	if (keycode == KEY_A)
		p->keys.a = 1;
	if (keycode == KEY_LEFT)
		p->keys.left = 1;
	if (keycode == KEY_RIGHT)
		p->keys.right = 1;
	if (keycode == KEY_SPACE)
		p->keys.jump = 1;
	if (keycode == KEY_E)
		p->keys.use = 1;
	if (keycode == MOUSE_LEFT)
		p->keys.attack = 1;
	return (0);
}

/*
** Handles key release events, clearing player input state
** Returns 0 to keep MLX event loop running
*/
int	handle_key_release(int keycode, t_game *game)
{
	t_player	*p;

	p = &game->p1;
	if (keycode == KEY_W)
		p->keys.w = 0;
	if (keycode == KEY_S)
		p->keys.s = 0;
	if (keycode == KEY_D)
		p->keys.d = 0;
	if (keycode == KEY_A)
		p->keys.a = 0;
	if (keycode == KEY_LEFT)
		p->keys.left = 0;
	if (keycode == KEY_RIGHT)
		p->keys.right = 0;
	if (keycode == KEY_SPACE)
		p->keys.jump = 0;
	if (keycode == KEY_E)
		p->keys.use = 0;
	if (keycode == MOUSE_LEFT)
		p->keys.attack = 0;
	return (0);
}

/*
** Handles mouse movement for player rotation
** Centers mouse if it moves too far from screen center
*/
int	handle_mouse_move(int x, int y, t_game *game)
{
	t_player	*p;
	t_vector	pos;
	t_vector	center;
	int			reset_needed;
	double		rotation;

	p = &game->p1;
	if (game->last_mouse.x == -1)
	{
		game->last_mouse = vector_create(x, y);
		return (0);
	}
	pos = vector_create(x, y);
	center = vector_create(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	reset_needed = (pos.x < WINDOW_WIDTH / 4 || pos.x > WINDOW_WIDTH * 3 / 4);
	if (!reset_needed)
	{
		rotation = (pos.x - game->last_mouse.x) * game->mouse_sensi;
		if (rotation > MAX_ROTATION)
			rotation = MAX_ROTATION;
		if (rotation < -MAX_ROTATION)
			rotation = -MAX_ROTATION;
		p->angle += (angle_t)(rotation * ANG90);
		p->angle &= ANGLEMASK;
		game->last_mouse = pos;
	}
	else
	{
		mlx_mouse_move(game->mlx, game->win, center.x, center.y);
		game->last_mouse = center;
	}
	return (0);
}
