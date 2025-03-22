/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:26:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/09 16:43:12 by hluiz-ma         ###   ########.fr       */
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

void	cleanup_animation(t_game *game, t_animation *anim)
{
	int	i;

	i = -1;
	while (++i < anim->frame_count)
	{
		if (anim->frames[i].img)
		{
			mlx_destroy_image(game->mlx, anim->frames[i].img);
			anim->frames[i].img = NULL;
		}
	}
	anim->frame_count = 0;
	anim->active = 0;
}

void	cleanup_menu(t_game *game)
{
	if (!game || !game->menu)
		return ;
	cleanup_animation(game, &game->menu->background);
	cleanup_animation(game, &game->menu->cube);
	cleanup_animation(game, &game->menu->start_btn.anim);
	cleanup_animation(game, &game->menu->exit_btn.anim);
	cleanup_animation(game, &game->menu->select_gun);
	free(game->menu);
	game->menu = NULL;
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
	cleanup_textures(game);
	cleanup_enemies(game);
	cleanup_door_system(game);
	cleanup_double_buffer(game);
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
