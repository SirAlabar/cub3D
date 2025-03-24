/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:47:44 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:47:45 by hluiz-ma         ###   ########.fr       */
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

static void	set_portal_gun_anim(t_game *game)
{
	t_texture	*tex[4];
	int			i;

	game->portal_system->gun.gun_anim = ft_calloc(4, sizeof(t_texture));
	if (!game->portal_system->gun.gun_anim)
		return (cleanup_game(game), exit(1));
	tex[0] = texture_create(game, PORTAL_GUN);
	tex[1] = texture_create(game, PORTAL_GUN2);
	tex[2] = texture_create(game, PORTAL_GUN3);
	tex[3] = texture_create(game, PORTAL_GUN4);
	i = -1;
	while (++i < 4)
	{
		if (!tex[i])
		{
			while (--i >= 0)
				free(tex[i]);
			free(game->portal_system->gun.gun_anim);
			ft_printf("Error\n: Failed to create portal gun textures\n");
			cleanup_game(game);
			exit(1);
		}
		game->portal_system->gun.gun_anim[i] = *tex[i];
		free(tex[i]);
	}
}

static void	init_portal_gun(t_game *game)
{
	game->portal_system->gun.blue_texture = load_portal_texture(game,
			PORTAL_BLUE_TEX);
	game->portal_system->gun.orange_texture = load_portal_texture(game,
			PORTAL_ORANGE_TEX);
	set_portal_gun_anim(game);
	game->portal_system->gun.last_fire_time = 0;
	game->portal_system->gun.cooldown = 500.0;
	game->portal_system->gun.active_portal = PORTAL_BLUE;
	game->portal_system->gun.current_frame = 0;
	game->portal_system->gun.is_firing = 0;
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
	portal->card = -1;
}

void	init_portal_system(t_game *game)
{
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
}
