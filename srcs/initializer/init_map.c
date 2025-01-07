/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:29:11 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/07 11:56:18 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_map(t_game *game)
{
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
	game->map.floor_color = 0x87CEEB;
	game->map.ceiling_color = 0x8B4513;
	game->map.player_start = '\0';
	game->map.player_pos = vector_create(0, 0);
	read_map(game);
}

char	**read_map(t_game *game)
{
	char	*line;
	int		i;
	
	game->map.height = count_lines(game);
	game->fd_map = open(game->map_path, O_RDONLY);
	line = get_next_line(game->fd_map);
	game->map.grid = ft_calloc(sizeof(char **), game->map.height + 1);
	i = 0;
	while(line)
	{
		while(line[0] != ' ' && line[0] != '1')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		game->map.grid[i] = ft_calloc(sizeof(char *), ft_strlen(line) + 1);
		if (i == game->map.height - 1)
			ft_strlcpy(game->map.grid[i], line, ft_strlen(line) + 1);
		else
			ft_strlcpy(game->map.grid[i], line, ft_strlen(line));
		free(line);
		i++;
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
	game->map.width = 8;
	print_map(game);
	return (NULL);
}

int	count_lines(t_game *game)
{
	char	*line;
	int		height;

	game->fd_map = open(game->map_path, O_RDWR);
	line = get_next_line(game->fd_map);
	height = 0;
	while(line)
	{
		while(line[0] != ' ' && line[0] != '1')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		free(line);
		line = get_next_line(game->fd_map);
		height++;
	}
	close(game->fd_map);
	return (height);
}

/*
void	set_map(t_game *game, char *map_path)
{
	int		fd;
	int		i;
	char	*str;

	fd = open(map_path, O_RDONLY);
	game->map.height = count_lines(map_path);
	game->map.grid = ft_calloc(sizeof(char **), game->map.rows + 1);
	i = 0;
	str = get_next_line(fd);
	while (str)
	{
		game->map.grid[i] = ft_calloc(sizeof(char *), ft_strlen(str) + 1);
		if (i == game->map.height - 1)
			ft_strlcpy(game->map.grid[i], str, ft_strlen(str) + 1);
		else
			ft_strlcpy(game->map.grid[i], str, ft_strlen(str));
		free(str);
		i++;
		str = get_next_line(fd);
	}
	close(fd);
	set_map_tiles(game);
}
*/
