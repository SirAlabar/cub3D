/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_img.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 20:44:28 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:49:01 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cleanup_gun(t_game *game)
{
	int	i;

	if (!game || !game->p1.gun_anim)
		return ;
	i = 0;
	while (i < 4)
	{
		if (game->p1.gun_anim[i].img)
		{
			mlx_destroy_image(game->mlx, game->p1.gun_anim[i].img);
			game->p1.gun_anim[i].img = NULL;
		}
		i++;
	}
	free(game->p1.gun_anim);
	game->p1.gun_anim = NULL;
}

void	cleanup_sky_floor(t_game *game)
{
	if (!game)
		return ;
	if (game->floor_path)
	{
		free(game->floor_path);
		game->floor_path = NULL;
	}
	if (game->skybox_path)
	{
		free(game->skybox_path);
		game->skybox_path = NULL;
	}
	if (game->mlx)
	{
		if (game->skybox.img)
		{
			mlx_destroy_image(game->mlx, game->skybox.img);
			game->skybox.img = NULL;
		}
		if (game->floor.img)
		{
			mlx_destroy_image(game->mlx, game->floor.img);
			game->floor.img = NULL;
		}
	}
}

void	cleanup_textures(t_game *game)
{
	if (!game)
		return ;
	if (game->north.img)
	{
		mlx_destroy_image(game->mlx, game->north.img);
		game->north.img = NULL;
	}
	if (game->south.img)
	{
		mlx_destroy_image(game->mlx, game->south.img);
		game->south.img = NULL;
	}
	if (game->east.img)
	{
		mlx_destroy_image(game->mlx, game->east.img);
		game->east.img = NULL;
	}
	if (game->west.img)
	{
		mlx_destroy_image(game->mlx, game->west.img);
		game->west.img = NULL;
	}
	if (game->door_system && game->door_system->door_texture.img)
		mlx_destroy_image(game->mlx, game->door_system->door_texture.img);
	cleanup_gun(game);
}

void	cleanup_door_system(t_game *game)
{
	if (!game || !game->door_system)
		return ;
	if (game->door_system->doors)
	{
		free(game->door_system->doors);
		game->door_system->doors = NULL;
	}
	free(game->door_system);
	game->door_system = NULL;
}

void	cleanup_double_buffer(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		if (game->img[0])
			mlx_destroy_image(game->mlx, game->img[0]);
		if (game->img[1])
			mlx_destroy_image(game->mlx, game->img[1]);
	}
	game->img[0] = NULL;
	game->img[1] = NULL;
}
