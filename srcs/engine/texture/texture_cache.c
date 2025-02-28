/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_cache.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/28 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Initialize texture cache in the game structure
** Returns false if memory allocation fails
*/
bool	init_texture_cache(t_game *game)
{
	if (!game)
		return (false);
		
	/* Allocate texture cache arrays */
	game->tex_cache = (t_texture_cache *)ft_calloc(1, sizeof(t_texture_cache));
	if (!game->tex_cache)
		return (false);
		
	return (true);
}

/*
** Create texture arrays in the texture cache
** Called after the map is loaded to set up the texture arrays
*/
bool	setup_texture_arrays(t_game *game)
{
	t_texture_cache *cache;
	
	if (!game || !game->map || !game->tex_cache)
		return (false);
		
	cache = game->tex_cache;
	cache->wall_textures = (t_wall_textureset *)ft_calloc(
			game->map->sidedef_count, sizeof(t_wall_textureset));
	cache->floor_textures = (t_texture **)ft_calloc(
			game->map->sector_count, sizeof(t_texture *));
	cache->ceiling_textures = (t_texture **)ft_calloc(
			game->map->sector_count, sizeof(t_texture *));
	if (!cache->wall_textures || !cache->floor_textures || 
		!cache->ceiling_textures)
	{
		clear_texture_cache(game);
		return (false);
	}
	cache->wall_count = game->map->sidedef_count;
	cache->floor_count = game->map->sector_count;
	cache->ceiling_count = game->map->sector_count;
	return (true);
}

/*
** Load a texture from file if not already loaded
** Returns NULL if loading fails
*/
t_texture	*load_texture(t_game *game, char *path)
{
	t_texture	*texture;

	if (!game || !path)
		return (NULL);
	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf("Error: Failed to load texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

/*
** Load all wall textures for a specific sidedef
** Returns true if at least one texture was loaded, false otherwise
*/
bool	load_wall_textures(t_game *game, int sidedef_index)
{
	t_sidedef			*sidedef;
	t_wall_textureset	*textureset;
	bool				loaded_any;
	
	if (!game || !game->map || !game->tex_cache)
		return (false);
	if (sidedef_index < 0 || sidedef_index >= game->map->sidedef_count)
		return (false);
	sidedef = &game->map->sidedefs[sidedef_index];
	textureset = &game->tex_cache->wall_textures[sidedef_index];
	loaded_any = false;
	if (sidedef->upper_texture && !textureset->upper)
	{
		textureset->upper = load_texture(game, sidedef->upper_texture);
		if (textureset->upper)
			loaded_any = true;
	}
	if (sidedef->middle_texture && !textureset->middle)
	{
		textureset->middle = load_texture(game, sidedef->middle_texture);
		if (textureset->middle)
			loaded_any = true;
	}
	if (sidedef->lower_texture && !textureset->lower)
	{
		textureset->lower = load_texture(game, sidedef->lower_texture);
		if (textureset->lower)
			loaded_any = true;
	}
	return (loaded_any);
}

/*
** Load floor texture for a specific sector
** Returns the loaded texture or NULL if loading fails
*/
t_texture	*load_floor_texture(t_game *game, int sector_index)
{
	t_sector	*sector;
	t_texture	*texture;
	
	if (!game || !game->map || !game->tex_cache)
		return (NULL);
	if (sector_index < 0 || sector_index >= game->map->sector_count)
		return (NULL);
	if (game->tex_cache->floor_textures[sector_index])
		return (game->tex_cache->floor_textures[sector_index]);
	sector = &game->map->sectors[sector_index];
	if (!sector->floor_texture)
		return (NULL);
	texture = load_texture(game, sector->floor_texture);
	if (texture)
		game->tex_cache->floor_textures[sector_index] = texture;
	return (texture);
}