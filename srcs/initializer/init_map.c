/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:29:11 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/06 20:50:14 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_map(t_game *game)
{
	game->map.grid = read_map(game);
	game->map.width = 0;
	game->map.height = 0;
	game->map.floor_color = 0x87CEEB;
	game->map.ceiling_color = 0x8B4513;
	game->map.player_start = '\0';
	game->map.player_pos = vector_create(0, 0);
}

char	**read_map(t_game *game)
{
	char	*line;
	char	**grid;
	
	game->map.width = 
	game->map.height = 
	while((line = get_next_line(game->fd_map)))
	{
		printf("%s", line);
	}
	return (NULL);
}

void	set_map(t_game *game, char *map_path)
{
	int		fd;
	int		i;
	char	*str;

	fd = open(map_path, O_RDONLY);
	game->map.rows = count_lines(map_path);
	game->map.data = ft_calloc(sizeof(char **), game->map.rows + 1);
	i = 0;
	str = get_next_line(fd);
	while (str)
	{
		game->map.data[i] = ft_calloc(sizeof(char *), ft_strlen(str) + 1);
		if (i == game->map.rows - 1)
			ft_strlcpy(game->map.data[i], str, ft_strlen(str) + 1);
		else
			ft_strlcpy(game->map.data[i], str, ft_strlen(str));
		free(str);
		i++;
		str = get_next_line(fd);
	}
	close(fd);
	set_map_tiles(game);
}
