/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:26:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 18:22:26 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cleanup_bsp_tree(t_bsp_tree *tree)
{
	if (!tree)
		return ;
	if (tree->root)
		free_bsp_node(tree->root);
	free(tree);
}

void	cleanup_doom_map(t_doom_map *map)
{
	int	i;

	if (!map)
		return ;
	i = -1;
	while (++i < map->sector_count)
	{
		if (map->sectors[i].linedefs)
			free(map->sectors[i].linedefs);
		if (map->sectors[i].floor_texture)
			free(map->sectors[i].floor_texture);
		if (map->sectors[i].ceiling_texture)
			free(map->sectors[i].ceiling_texture);
	}
	free(map);
}

void	cleanup_textures(t_game *game)
{
	if (game->mlx)
	{
//		cleanup_skybox(game);
//		cleanup_sector_textures(game);
//		cleanup_weapon_textures(game);
	}
}

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
//	cleanup_textures(game);
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
//	cleanup_double_buffer(game);
	if (game->mlx)
		cleanup_mlx(game->mlx);
	cleanup_doom_map(game->map);
	cleanup_bsp_tree(game->bsp_tree);
//	if (game->fd_map != -1)
//		close(game->fd_map);
	free(game);
}
