

#include <cub3d.h>

/*
** Load ceiling texture for a specific sector
** Returns the loaded texture or NULL if loading fails
*/
t_texture	*load_ceiling_texture(t_game *game, int sector_index)
{
	t_sector	*sector;
	t_texture	*texture;
	
	if (!game || !game->map || !game->tex_cache)
		return (NULL);
	if (sector_index < 0 || sector_index >= game->map->sector_count)
		return (NULL);
	if (game->tex_cache->ceiling_textures[sector_index])
		return (game->tex_cache->ceiling_textures[sector_index]);
	sector = &game->map->sectors[sector_index];
	if (!sector->ceiling_texture)
		return (NULL);
	texture = load_texture(game, sector->ceiling_texture);
	if (texture)
		game->tex_cache->ceiling_textures[sector_index] = texture;
	return (texture);
}

/*
** Get wall texture for a specific part (upper, middle, lower)
** Loads the texture if not already loaded
** Returns NULL if the texture cannot be loaded
*/
t_texture	*get_wall_texture(t_game *game, int linedef_index, bool front_side, int part)
{
	t_linedef			*linedef;
	int					sidedef_index;
	t_wall_textureset	*textureset;
	
	if (!game || !game->map || !game->tex_cache)
		return (NULL);
	if (linedef_index < 0 || linedef_index >= game->map->linedef_count)
		return (NULL);
	linedef = &game->map->linedefs[linedef_index];
	if (front_side)
		sidedef_index = linedef->front_sidedef;
	else
		sidedef_index = linedef->back_sidedef;
	if (sidedef_index < 0 || sidedef_index >= game->map->sidedef_count)
		return (NULL);
	load_wall_textures(game, sidedef_index);
	textureset = &game->tex_cache->wall_textures[sidedef_index];
	if (part == WALL_UPPER)
		return (textureset->upper);
	else if (part == WALL_MIDDLE)
		return (textureset->middle);
	else if (part == WALL_LOWER)
		return (textureset->lower);
	return (NULL);
}

/*
** Preloads all textures used in the map
** This ensures no loading delays during gameplay
*/
bool	preload_map_textures(t_game *game)
{
	int		i;
	int		loaded_count;
	
	if (!game || !game->map || !game->tex_cache)
		return (false);
	loaded_count = 0;
	if (!setup_texture_arrays(game))
		return (false);
	if (game->map->skybox_path)
	{
		game->skybox_tex = load_texture(game, game->map->skybox_path);
		if (game->skybox_tex)
			loaded_count++;
	}
	for (i = 0; i < game->map->sidedef_count; i++)
	{
		if (load_wall_textures(game, i))
			loaded_count++;
	}
	for (i = 0; i < game->map->sector_count; i++)
	{
		if (load_floor_texture(game, i))
			loaded_count++;
	}
	for (i = 0; i < game->map->sector_count; i++)
	{
		if (load_ceiling_texture(game, i))
			loaded_count++;
	}
	ft_printf("Preloaded %d textures for the map\n", loaded_count);
	return (true);
}
