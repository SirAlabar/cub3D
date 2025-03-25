/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:26:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 19:54:45 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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

static void	cleanup_portal_gun(t_game *game)
{
	int	i;

	if (game->portal_system->gun.blue_texture)
	{
		mlx_destroy_image(game->mlx,
			game->portal_system->gun.blue_texture->img);
		free(game->portal_system->gun.blue_texture);
	}
	if (game->portal_system->gun.orange_texture)
	{
		mlx_destroy_image(game->mlx,
			game->portal_system->gun.orange_texture->img);
		free(game->portal_system->gun.orange_texture);
	}
	if (game->portal_system->gun.gun_anim)
	{
		i = -1;
		while (++i < 4)
		{
			if (game->portal_system->gun.gun_anim[i].img)
				mlx_destroy_image(game->mlx,
					game->portal_system->gun.gun_anim[i].img);
		}
		free(game->portal_system->gun.gun_anim);
	}
}

void	cleanup_portal_system(t_game *game)
{
	if (!game || !game->portal_system)
		return ;
	cleanup_portal_gun(game);
	free(game->portal_system);
	game->portal_system = NULL;
}

void	cleanup_enemies(t_game *game)
{
	t_enemy_list	*current;
	t_enemy_list	*next;

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
	cleanup_menu(game);
	cleanup_textures(game);
	cleanup_enemies(game);
	cleanup_door_system(game);
	cleanup_double_buffer(game);
	cleanup_portal_system(game);
	cleanup_sounds(game);
	cleanup_sky_floor(game);
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		cleanup_mlx(game->mlx);
	cleanup_map(game);
	if (game->fd_map != -1)
		close(game->fd_map);
	free(game);
}
