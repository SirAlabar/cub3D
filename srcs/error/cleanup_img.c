/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_img.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 20:44:28 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:58:36 by hluiz-ma         ###   ########.fr       */
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
