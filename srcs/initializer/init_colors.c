/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_colors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:10:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/08 18:19:30 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_colors(t_game *game)
{
	char	*line;

	game->fd_map = open(game->map_path, O_RDONLY);
	line = get_next_line(game->fd_map);
	while (line)
	{
		while (line && line[0] != 'F' && line[0] != 'C')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		if (line && line[0] == 'F')
			printf("found: %s\n", line);
		else if (line && line[0] == 'C')
			printf("found: %s\n", line);
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}
