/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:31:22 by marsoare          #+#    #+#             */
/*   Updated: 2025/04/12 15:57:18 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	contains_valid_characters(t_game *game)
{
	int	i;
	int	j;
	int	current_line_length;

	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		current_line_length = 0;
		while (game->map.grid[i][current_line_length] != '\0')
			current_line_length++;
		while (++j < current_line_length)
		{
			if (!ft_strchr(VALID_MAP_CHARS, game->map.grid[i][j])
					&& game->map.grid[i][j] != '\n')
			{
				return (false);
			}
		}
	}
	return (true);
}
