/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:14 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:55:16 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	engine_prepare_frame(t_game *game)
{
	if (!game || !game->mlx)
		return ;
	if (game->img)
	{
		mlx_destroy_image(game->mlx, game->img);
		game->img = NULL;
	}
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return ;
	game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel,
			&game->line_length, &game->endian);
}

void	animate_gun(t_game *game)
{
	int	center_x;
	int	bottom_y;

	center_x = (WINDOW_WIDTH / 2) - (game->p1.gun_anim[game->p1.current_frame].width / 2);
	bottom_y = WINDOW_HEIGHT - game->p1.gun_anim[game->p1.current_frame].height;

	game->p1.current_frame = (game->p1.current_frame + 1) % 3;
	mlx_put_image_to_window(game->mlx, game->win,
		game->p1.gun_anim[game->p1.current_frame].img, center_x, bottom_y);
}

int	engine_render_frame(t_game *game)
{
	int		i;
	t_ray	rays[WINDOW_WIDTH];

	animate_gun(game);
	engine_prepare_frame(game);
	draw_background(game);
	cast_rays(game, rays);
	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		draw_wall(game, &rays[i], i);
	}
	handle_movement(game);
	update_weapon_animation(game);
	draw_weapon(game);	
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
