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
** Initialize player state with position and movement parameters
** Sets up initial sector location and movement variables
*/
static bool	init_player(t_game *game)
{
	t_player	*p;
	t_thing		*thing;
	t_bsp_node	*node;

	p = &game->p1;
	thing = find_player_thing(game->map);
	if (!thing)
		return (ft_putendl_fd("Error: No player start found", 2), false);
	ft_bzero(p, sizeof(t_player));
	p->pos.x = thing->pos.x;
	p->pos.y = thing->pos.y;
	p->angle = thing->angle;
	p->health = 100;
	p->armor = 0;
	node = find_bsp_node(game->bsp_tree->root, p->pos.x, p->pos.y);
	if (!node || !node->is_leaf || !node->sector)
		return (false);
	p->sector = node->sector;
	p->z = p->sector->floor_height;
	p->view_z = p->z + PLAYER_VIEW_HEIGHT;
	p->on_ground = 1;
	return (true);
}

/*
** Initialize weapon textures and animation frames
** Loads gun sprites for player weapon
*/
static bool	init_textures(t_game *game)
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

/*
** Main game initialization function
** Sets up subsystems after map and BSP are loaded
*/
bool	init_game(t_game *game)
{
	if (!game)
		return (false);
	game->tables = init_fixed_tables_8192();
	if (!game->tables)
		return (false);
	if (!init_player(game))
		return (false);
	if (!init_textures(game))
		return (false);
	game->mouse_sensi = float_to_fixed32(0.003);
	game->last_mouse = vector_create(-1, -1);
	return (true);
}