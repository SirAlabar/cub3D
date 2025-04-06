/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:11:06 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/26 14:11:26 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	*validate_player_count(t_game *game)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = -1;
	while (++i < game->map.height)
	{
		j = 0;
		while (game->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S' ||
				game->map.grid[i][j] == 'W' || game->map.grid[i][j] == 'E')
			{
				(player_count++);
				game->map.player_start = game->map.grid[i][j];
				game->map.player_pos = vector_create(j, i);
			}
			j++;
		}
	}
	if (player_count != 1)
		return (printf("Error\nMap must contain one player.\n"),
			cleanup_game(game), exit(1), NULL);
	return (NULL);
}
