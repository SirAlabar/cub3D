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

static void handle_mouse_rotation(t_game *game, t_vector pos)
{
    // Previne estado inicial
    if (game->last_mouse.x == -1) {
        game->last_mouse = pos;
        return;
    }

    // Calcula rotação em fixed-point
    t_fixed32 mouse_diff = int_to_fixed32(pos.x - game->last_mouse.x);
    t_fixed32 rotation = fixed32_mul(mouse_diff, float_to_fixed32(0.003));
    
    ft_printf("\n=== Mouse Rotation Debug ===\n");
    ft_printf("Current Mouse X: %d\n", (int)pos.x);
    ft_printf("Last Mouse X: %d\n", (int)game->last_mouse.x);
    ft_printf("Mouse Diff (fixed): %d\n", mouse_diff);
    ft_printf("Rotation (fixed): %d\n", rotation);

    // Limita rotação
    t_fixed32 max_rotation = float_to_fixed32(0.5);
    rotation = fix_min(rotation, max_rotation);
    rotation = fix_max(rotation, -max_rotation);

    ft_printf("Limited Rotation (fixed): %d\n", rotation);
    
    // Converte rotação para incremento de ângulo
    t_fixed32 angle_increment = fixed32_mul(rotation, int_to_fixed32(ANG90));
    
    ft_printf("Angle Increment: %u\n", angle_increment);
    ft_printf("Before Rotation - Player Angle: %u\n", game->p1.angle);

    game->p1.angle += angle_increment;
    game->p1.angle &= ANGLEMASK;
    
    ft_printf("After Rotation - Player Angle: %u\n", game->p1.angle);
    
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
