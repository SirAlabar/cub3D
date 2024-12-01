/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:24:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/11/30 21:28:49 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	validate_map(t_game *game, char **argv)
{
	(void) game;
	printf("checking map: %s\n", argv[0]);
	printf("checking map: %s\n", argv[1]);
	return (true);
}

int	check_wall(t_map map)
{
	int	x;
	int	y;

	x = 0;
	while (x < map.width)
	{
		if (map.grid[0][x] != '1' || map.grid[map.height - 1][x] != '1')
			return (0);
		x++;
	}
	y = 0;
	while (y < map.height)
	{
		if (map.grid[y][0] != '1' || map.grid[y][map.width - 1] != '1')
			return (0);
		y++;
	}
	return (1);
}

int	map_caracter(t_game *game)
{
	int		i;
	char	current_char;

	i = -1;
	while (game->map.grid[++i] != NULL)
	{
		current_char = *game->map.grid[i];
		if (!ft_strchr(VALID_MAP_CHARS, current_char) && current_char != '\n')
			return (0);
	}
	return (1);
}
