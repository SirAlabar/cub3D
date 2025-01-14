/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:53:43 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:53:46 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_map(t_game *game)
{
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
	game->map.floor_color = 9134906;
	game->map.ceiling_color = 11272191;
	init_colors(game);
	game->map.player_start = '\0';
	game->map.player_pos = vector_create(0, 0);
	read_map(game);
}

int	get_map_width(t_game *game)
{
	int	width;
	int	tmp;
	int	height;
	int	i;

	width = 0;
	height = game->map.height;
	i = 0;
	tmp = 0;
	while (height)
	{
		if (game->map.grid[i])
			tmp = ft_strlen(game->map.grid[i]);
		if (tmp > width)
			width = tmp;
		height--;
		i++;
	}
	return (width);
}

char	**read_map(t_game *game)
{
	game->map.height = count_lines(game);
	if (game->map.height <= 0)
		return (NULL);
	game->map.grid = ft_calloc(sizeof(char *), game->map.height + 1);
	if (!game->map.grid)
		return (NULL);
	set_grid(game);
	game->map.width = get_map_width(game);
	return (game->map.grid);
}

void	set_grid(t_game *game)
{
	char	*line;
	int		i;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	i = 0;
	while (line && line[0] != '\0')
	{
		while (line && line[0] != ' ' && line[0] != '1')
			(free(line), line = get_next_line(game->fd_map));
		if (!line)
			break ;
		game->map.grid[i] = ft_calloc(sizeof(char *), ft_strlen(line) + 1);
		if (i == game->map.height - 1)
			ft_strlcpy(game->map.grid[i++], line, ft_strlen(line) + 1);
		else
			ft_strlcpy(game->map.grid[i++], line, ft_strlen(line));
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}

int	count_lines(t_game *game)
{
	char	*line;
	int		height;

	game->fd_map = open(game->map_path, O_RDWR);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	height = 0;
	while (line)
	{
		while (line && line[0] != ' ' && line[0] != '1')
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
