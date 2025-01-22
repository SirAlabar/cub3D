/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:26:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:08:59 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cleanup_gun(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->p1.gun_anim[i].img)
		{
			mlx_destroy_image(game->mlx, game->p1.gun_anim[i].img);
		}
		i++;
	}
	free(game->p1.gun_anim);
	game->p1.gun_anim = NULL;
}

void	cleanup_textures(t_game *game)
{
	if (game->north.img)
		mlx_destroy_image(game->mlx, game->north.img);
	if (game->south.img)
		mlx_destroy_image(game->mlx, game->south.img);
	if (game->east.img)
		mlx_destroy_image(game->mlx, game->east.img);
	if (game->west.img)
		mlx_destroy_image(game->mlx, game->west.img);
	cleanup_gun(game);
}

void	cleanup_door_system(t_game *game)
{
	if (!game->door_system)
		return ;
	if (game->door_system->door_texture.img)
		mlx_destroy_image(game->mlx, game->door_system->door_texture.img);
	if (game->door_system->doors)
		free(game->door_system->doors);
	free(game->door_system);
	game->door_system = NULL;
}

void	cleanup_map(t_game *game)
{
	int	i;

	i = 0;
	if (game->map.grid)
	{
		while (i < game->map.height)
		{
			if (game->map.grid[i])
				free(game->map.grid[i]);
			i++;
		}
		free(game->map.grid);
	}
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	cleanup_textures(game);
	cleanup_door_system(game);
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
	if (game->img && game->mlx)
		mlx_destroy_image(game->mlx, game->img);
	if (game->mlx)
		cleanup_mlx(game->mlx);
	cleanup_map(game);
	if (game->fd_map != -1)
		close(game->fd_map);
	free(game);
}
