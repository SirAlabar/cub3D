/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:53:43 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 22:57:56 by marvin           ###   ########.fr       */
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
	int	i;

	width = 0;
	i = 0;
	while (i < game->map.height)
	{
		tmp = ft_strlen(game->map.grid[i]);
		if (tmp > width)
			width = tmp;
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
	size_t	len;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	i = 0;
	while (line && i < game->map.height)
	{
		while (line && line[0] != ' ' && line[0] != '1')
		{
			free(line);
			line = get_next_line(game->fd_map);
			if (!line)
				break ;
		}
		if (!line)
			break ;
		len = ft_strlen(line);
		game->map.grid[i] = ft_calloc(len + 1, sizeof(char));
		if (!game->map.grid[i])
		{
			free(line);
			read_error(game);
			return ;
		}
		ft_strlcpy(game->map.grid[i], line, len + 1);
		if (game->map.grid[i][len - 1] == '\n')
			game->map.grid[i][len - 1] = '\0';
		free(line);
		i++;
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
		while (line[0] != ' ' && line[0] != '1')
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
