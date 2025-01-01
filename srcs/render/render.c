/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 14:57:09 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/01 16:56:33 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void put_pixel_game(t_game *game, int x, int y, int color)
{
    char *dst;

    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return;
    dst = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void draw_floor_ceiling(t_game *game)
{
    int x;
    int y;

    if (!game || !game->img)
    {
        printf("Game or image pointer is NULL\n");
        return;
    }
    y = 0;
    while(y < WINDOW_HEIGHT)
    {
        x = 0;
        while(x < WINDOW_WIDTH)
        {
            if (y < WINDOW_HEIGHT / 2)
                put_pixel_game(game, x, y, 0x87CEEB);
            else
                put_pixel_game(game, x, y, 0x8B4513);
            x++;
        }
        y++;
    }
}
void render_frame(t_game *game)
{
    if (game->img && game->mlx)
    {
        mlx_destroy_image(game->mlx, game->img);
        game->img = NULL;
    }
    
    game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,&game->line_length, &game->endian);
    
    draw_floor_ceiling(game);
    
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
