/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:10:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/05 15:11:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_texture	*load_portal_texture(t_game *game, char *path)
{
	t_texture	*texture;

	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf("Error: Failed to load portal texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

static void	init_portal_gun(t_game *game)
{
	game->portal_system->gun.blue_texture = load_portal_texture(game, 
		PORTAL_BLUE_TEX);
	game->portal_system->gun.orange_texture = load_portal_texture(game, 
		PORTAL_ORANGE_TEX);
	game->portal_system->gun.gun_texture = load_portal_texture(game,
		PORTAL_GUN);
	game->portal_system->gun.last_fire_time = 0;
	game->portal_system->gun.cooldown = 500.0;
	game->portal_system->gun.active_portal = PORTAL_BLUE;
}

static void	init_portal_wall(t_portal_wall *portal)
{
    portal->position = vector_i_create(-1, -1);
	portal->normal = vector_create(0, 0);
	portal->offset = 0.0;
	portal->type = PORTAL_INACTIVE;
	portal->state = PORTAL_CLOSED;
	portal->animation = 0.0;
	portal->active = false;
	portal->timer = 0.0;
	portal->linked_portal = NULL;
}

void	init_portal_system(t_game *game)
{
    printf("DEBUG: Initializing portal system\n");
	game->portal_system = ft_calloc(1, sizeof(t_portal_system));
	if (!game->portal_system)
	{
		ft_printf("Error: Failed to allocate portal system\n");
		return ;
	}
	init_portal_gun(game);
	init_portal_wall(&game->portal_system->blue_portal);
	init_portal_wall(&game->portal_system->orange_portal);
	game->portal_system->blue_portal.type = PORTAL_BLUE;
	game->portal_system->orange_portal.type = PORTAL_ORANGE;
	game->portal_system->portal_active = false;
	game->portal_system->last_teleport_time = 0;
    printf("DEBUG: Portal system initialized successfully\n");
}

void	cleanup_portal_system(t_game *game)
{
	if (!game || !game->portal_system)
		return ;
	if (game->portal_system->gun.blue_texture)
	{
		mlx_destroy_image(game->mlx, game->portal_system->gun.blue_texture->img);
		free(game->portal_system->gun.blue_texture);
	}
	if (game->portal_system->gun.orange_texture)
	{
		mlx_destroy_image(game->mlx, game->portal_system->gun.orange_texture->img);
		free(game->portal_system->gun.orange_texture);
	}
	if (game->portal_system->gun.gun_texture)
	{
		mlx_destroy_image(game->mlx, game->portal_system->gun.gun_texture->img);
		free(game->portal_system->gun.gun_texture);
	}
	free(game->portal_system);
	game->portal_system = NULL;
}
