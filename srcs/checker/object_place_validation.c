/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/15 20:27:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_near_wall(t_game *game, t_vector pos)
{
	int		x;
	int		y;
	//double	dist;

	x = (int)pos.x;
	y = (int)pos.y;
	//dist = MIN_SPACE_FROM_WALL;

	return (game->map.grid[y][x + 1] == '1'
		|| game->map.grid[y][x - 1] == '1'
		|| game->map.grid[y + 1][x] == '1'
		|| game->map.grid[y - 1][x] == '1');
}

bool	is_away_from_wall(t_game *game, t_vector pos)
{
	int		x;
	int		y;

	x = (int)pos.x;
	y = (int)pos.y;

	return (game->map.grid[y][x + 1] == '0'
		&& game->map.grid[y][x - 1] == '0'
		&& game->map.grid[y + 1][x] == '0'
		&& game->map.grid[y - 1][x] == '0');
}

bool	is_near_door(t_game *game, t_vector pos)
{
	int		x;
	int		y;

	x = (int)pos.x;
	y = (int)pos.y;

	return (game->map.grid[y][x + 1] == 'D'
		|| game->map.grid[y][x - 1] == 'D'
		|| game->map.grid[y + 1][x] == 'D'
		|| game->map.grid[y - 1][x] == 'D');
}

bool	check_object_spacing(t_game *game, t_vector pos)
{
	int		i;
	double	dist;

	i = 0;
	while (i < game->map.num_sprites)
	{
		dist = sqrt(pow(pos.x - game->map.sprites[i].pos.x, 2)
				+ pow(pos.y - game->map.sprites[i].pos.y, 2));
		if (dist < MIN_SPACE_BETWEEN_OBJECTS)
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_object_position(t_game *game, t_vector pos,
		t_placement_type placement)
{
	int	x;
	int	y;

	x = (int)pos.x;
	y = (int)pos.y;
	if (x < 1 || x >= game->map.width - 1 || y < 1 || y >= game->map.height - 1)
		return (false);
	if (game->map.grid[y][x] != '0')
		return (false);
	if (is_near_door(game, pos))
		return (false);
	if (!check_object_spacing(game, pos))
		return (false);
	if (placement == PLACEMENT_WALL && !is_near_wall(game, pos))
		return (false);
	if (placement == PLACEMENT_AWAY && !is_away_from_wall(game, pos))
		return (false);
	return (true);
}

t_vector	get_random_position(t_game *game, t_placement_type placement)
{
	t_vector	pos;
	int			attempts;

	attempts = 0;
	while (attempts < 100)
	{
		pos.x = 1.0 + ((double)rand() / RAND_MAX) * (game->map.width - 2);
		pos.y = 1.0 + ((double)rand() / RAND_MAX) * (game->map.height - 2);
		if (is_valid_object_position(game, pos, placement))
			return (pos);
		attempts++;
	}
	return ((t_vector){-1, -1});
}
