/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:02:20 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/12 15:09:53 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Find player starting position in map thing data
** Returns pointer to player thing or NULL if not found
*/
static t_thing	*find_player_thing(t_doom_map *map)
{
	int	i;

	i = 0;
	while (i < map->thing_count)
	{
		if (map->things[i].type == 1)
			return (&map->things[i]);
		i++;
	}
	return (NULL);
}

static int find_player_sector(t_doom_map *map, t_fixed_vec32 pos)
{
    int i;
    int j;
    bool inside;

    i = 0;
    while (i < map->sector_count)
    {
        inside = true;
        j = 0;
        while (j < map->sectors[i].linedef_count)
        {
            t_linedef *line = &map->linedefs[map->sectors[i].linedefs[j]];
            t_fixed_vec32 start = map->vertices[line->vertex1].pos;
            t_fixed_vec32 end = map->vertices[line->vertex2].pos;
            t_fixed_vec32 line_vec = vector_sub_fixed32(end, start);
            t_fixed_vec32 point_vec = vector_sub_fixed32(pos, start);
            t_fixed32 cross = fixed32_sub(
                fixed32_mul(line_vec.x, point_vec.y),
                fixed32_mul(line_vec.y, point_vec.x)
            );
            if (cross < 0)
            {
                inside = false;
                break;
            }
            j++;
        }
        if (inside)
            return (i);
        i++;
    }
    return (0);
}
/*
** Initialize player state with position and movement parameters
** Sets up initial sector location and movement variables
*/
bool    init_player(t_game *game)
{
    t_thing     *thing;

    thing = find_player_thing(game->map);
    if (!thing)
        return (ft_putendl_fd("Error: No player start found", 2), false);
    ft_bzero(&game->p1, sizeof(t_player));
    game->p1.pos = fixed_vec_create(thing->pos.x, thing->pos.y);
    game->p1.momx = 0;
    game->p1.momy = 0;
    game->p1.momz = 0;
    game->p1.angle = thing->angle;
    game->p1.on_ground = 1;
    ft_bzero(&game->p1.cmd, sizeof(t_cmd));
    ft_bzero(&game->p1.keys, sizeof(t_keys));
    game->p1.health = 100;
    game->p1.armor = 0;
    game->p1.current_frame = 0;
    game->p1.is_firing = 0;
    game->p1.last_fire = 0;
	int sector_index = find_player_sector(game->map, game->p1.pos);
    game->p1.sector = &game->map->sectors[sector_index];
    if (!game->p1.sector)
    {
        ft_putendl_fd("Error: Player not in any sector", 2);
        return (false);
    }
    game->p1.z = game->p1.sector->floor_height;
    game->p1.view_z = game->p1.z + PLAYER_VIEW_HEIGHT;
    return (true);
}

/*
** Initialize weapon textures and animation frames
** Loads gun sprites for player weapon
*/
bool	init_textures(t_game *game)
{
	t_texture	*temp;

	game->p1.gun_anim = ft_calloc(4, sizeof(t_texture));
	if (!game->p1.gun_anim)
		return (false);
	temp = texture_create(game, GUN_F1);
	if (!temp)
		return (false);
	game->p1.gun_anim[0] = *temp;
	temp = texture_create(game, GUN_F2);
	if (!temp)
		return (false);
	game->p1.gun_anim[1] = *temp;
	temp = texture_create(game, GUN_F3);
	if (!temp)
		return (false);
	game->p1.gun_anim[2] = *temp;
	temp = texture_create(game, GUN_F4);
	if (!temp)
		return (false);
	game->p1.gun_anim[3] = *temp;
	return (true);
}

/*
** Main game initialization function
** Sets up subsystems after map and BSP are loaded
*/
bool	init_game(t_game *game)
{
	if (!game)
		return (false);
    game->fixed_tables = init_fixed_tables_8192();
    if (!game->fixed_tables)
	{
        return (false);
	}
	if (!init_player(game))
		return (false);
	if (!init_textures(game))
		return (false);
	game->mouse_sensi = float_to_fixed32(0.003);
	game->last_mouse = vector_create(-1, -1);
	return (true);
}