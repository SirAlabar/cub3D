/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:26:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/25 11:58:25 by marsoare         ###   ########.fr       */
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

void    cleanup_enemies(t_game *game)
{
	t_enemy_list    *current;
	t_enemy_list    *next;

	if (!game->enemies)
		return ;
	current = game->enemies;
	while (current)
	{
		next = current->next;
		if (current->enemy.texture && current->enemy.texture->img)
			mlx_destroy_image(game->mlx, current->enemy.texture->img);
		if (current->enemy.texture)
			free(current->enemy.texture);
		free(current);
		current = next;
	}
	game->enemies = NULL;
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	cleanup_textures(game);
	cleanup_enemies(game);
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
