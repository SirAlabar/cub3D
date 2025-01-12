/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:01:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/12 14:15:56 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

#include "cub3d.h"

static bool	recursive_flood_fill(t_game *game, int x, int y)
{
	int	current_line_length;

	if (x < 0 || y < 0 || x >= game->map.height || y >= game->map.width
		|| game->map.grid[x][y] == '1' || game->map.grid[x][y] == 'X')
		return (true);
	current_line_length = 0;
	while (game->map.grid[x][current_line_length] != '\0')
		current_line_length++;
	if (y >= current_line_length)
		return (false);
	if (game->map.grid[x][y] == ' ')
		return (false);
	if (game->map.grid[x][y] == '0' && (x == 0 || y == 0
			|| x == game->map.height - 1 || y >= current_line_length - 1))
		return (false);
	game->map.grid[x][y] = 'X';
	if (!recursive_flood_fill(game, x - 1, y)
		|| !recursive_flood_fill(game, x + 1, y)
		|| !recursive_flood_fill(game, x, y - 1)
		|| !recursive_flood_fill(game, x, y + 1))
		return (false);
	return (true);
}

static bool	find_start_position(t_game *game)
{
	int	i;
	int	j;
	int	current_line_length;

	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
		{
			current_line_length = 0;
			while (game->map.grid[i][current_line_length] != '\0')
				current_line_length++;
			if (j >= current_line_length)
				continue;
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S'
				|| game->map.grid[i][j] == 'E' || game->map.grid[i][j] == 'W')
			{
				game->p1.pos.x = i;
				game->p1.pos.y = j;
				return (true);
			}
		}
	}
	return (false);
}

bool	flood_fill(t_game *game)
{
	if (!find_start_position(game))
	{
		printf("Error: Player starting position not found.\n");
		return (false);
	}
	if (!recursive_flood_fill(game, game->p1.pos.x, game->p1.pos.y))
	{
		printf("Error: Map is not fully enclosed.\n");
		return (false);
	}
	return (true);
}

bool	is_map_valid(t_game *game)
{
	if (!flood_fill(game))
		return (false);
	return (true);
}
