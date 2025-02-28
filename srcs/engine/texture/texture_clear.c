/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:45:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:45:01 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	texture_destroy(t_texture **texture, void *mlx)
{
	if (*texture)
	{
		if ((*texture)->img)
		{
			mlx_destroy_image(mlx, (*texture)->img);
			(*texture)->img = NULL;
		}
		free(*texture);
		*texture = NULL;
	}
}

void	clear_texture_cache(t_game *game)
{
	int				i;
	t_texture_cache	*cache;
	
	if (!game || !game->tex_cache)
		return;
		
	cache = game->tex_cache;

	if (cache->wall_textures)
	{
		for (i = 0; i < cache->wall_count; i++)
		{
			if (cache->wall_textures[i].upper)
				texture_destroy(&cache->wall_textures[i].upper, game->mlx);
			if (cache->wall_textures[i].middle)
				texture_destroy(&cache->wall_textures[i].middle, game->mlx);
			if (cache->wall_textures[i].lower)
				texture_destroy(&cache->wall_textures[i].lower, game->mlx);
		}
		free(cache->wall_textures);
	}
	if (cache->floor_textures)
	{
		for (i = 0; i < cache->floor_count; i++)
		{
			if (cache->floor_textures[i])
				texture_destroy(&cache->floor_textures[i], game->mlx);
		}
		free(cache->floor_textures);
	}
	if (cache->ceiling_textures)
	{
		for (i = 0; i < cache->ceiling_count; i++)
		{
			if (cache->ceiling_textures[i])
				texture_destroy(&cache->ceiling_textures[i], game->mlx);
		}
		free(cache->ceiling_textures);
	}
	if (game->skybox_tex)
		texture_destroy(&game->skybox_tex, game->mlx);
	free(game->tex_cache);
	game->tex_cache = NULL;
}