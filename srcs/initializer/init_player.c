/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:55:26 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 21:55:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	set_player_orientation(t_game *game)
{
	const int	x = (int)game->p1.pos.x;
	const int	y = (int)game->p1.pos.y;

	if (game->map.grid[x][y] == 'N')
	{
		game->p1.dir = vector_create(0, -1);
		game->p1.plane = vector_create(0.66, 0);
	}
	else if (game->map.grid[x][y] == 'S')
	{
		game->p1.dir = vector_create(0, 1);
		game->p1.plane = vector_create(-0.66, 0);
	}
	else if (game->map.grid[x][y] == 'E')
	{
		game->p1.dir = vector_create(1, 0);
		game->p1.plane = vector_create(0, -0.66);
	}
	else if (game->map.grid[x][y] == 'W')
	{
		game->p1.dir = vector_create(-1, 0);
		game->p1.plane = vector_create(0, 0.66);
	}
}

void    set_gun(t_game *game)
{
	t_texture    *tex[3];
	int          i;

	game->p1.gun_anim = ft_calloc(3, sizeof(t_texture));
	if (!game->p1.gun_anim)
		return (cleanup_game(game), exit(1));
	tex[0] = texture_create(game, GUN_F1);
	tex[1] = texture_create(game, GUN_F2);
	tex[2] = texture_create(game, GUN_F3);
	tex[3] = texture_create(game, GUN_F3);	
	i = -1;
	while (++i < 4)
	{
		if (!tex[i])
		{
			while (--i >= 0)
				free(tex[i]);
			free(game->p1.gun_anim);
			ft_printf("Error\n: Failed to create gun textures\n");
			cleanup_game(game);
			exit(1);
		}
		game->p1.gun_anim[i] = *tex[i];
		free(tex[i]);
	}
}

void	init_player(t_game *game)
{
	game->p1.pos = vector_create(game->p1.pos.x + 0.5f, game->p1.pos.y + 0.5f);
	set_player_orientation(game);
	game->p1.move_speed = MOVE_SPEED;
	game->p1.rot_speed = ROTATION_SPEED;
	game->p1.keys = (t_keys){0, 0, 0, 0, 0, 0};
    game->p1.current_frame = 0;
    game->p1.is_firing = 0;
    game->p1.last_step = get_time_ms();
    game->p1.last_fire = 0;	
	set_gun(game);
}
