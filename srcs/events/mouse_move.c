/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:20:40 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 17:07:55 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int mouse_move(t_game *game, t_vector pos)
{
    t_vector center;
    double   rotation;

    printf("Mouse move called: pos.x = %f, pos.y = %f\n", pos.x, pos.y);
    printf("Last mouse: x = %f, y = %f\n", game->last_mouse.x, game->last_mouse.y);

    if (game->last_mouse.x == -1)
    {
        game->last_mouse = pos;
        return (0);
    }

    rotation = (pos.x - game->last_mouse.x) * game->mouse_sensi;
    rotate_player(game, -rotation);
    game->last_mouse = pos;

    center = vector_create(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    if (pos.x < WINDOW_WIDTH / 4 || pos.x > WINDOW_WIDTH * 3 / 4)
    {
        mlx_mouse_move(game->mlx, game->win, center.x, center.y);\
        game->last_mouse = center;
    }
    return (0);
}
