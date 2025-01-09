/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:08:26 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/09 19:04:21 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	ft_istexture(char c)
{
	int	i = 0;
	char	*arr = "NSWE";
	while (arr[i])
	{
		if (c == arr[i])
			return (true);
		i++;
	}
	return (false);
}

void	*set_texture(t_texture *texture, char *path)
{
	int	i;

	i = 2;
	while (path && path[i] && ft_isspace(path[i]))
		i++;
	texture->path = &path[i];
	printf("Norht inside path: %s\n", texture->path);
	return (NULL);
}

void	*split_textures(t_game *game, char *line)
{
	(void) game;
	if (line && ft_strncmp(line, "NO", 2) == 0)
	{
		set_texture(&game->north, line);
	}
	if (line && ft_strncmp(line, "SO", 2) == 0)
	{
		set_texture(&game->south, line);
	}
	if (line && ft_strncmp(line, "WE", 2) == 0)
	{
		set_texture(&game->west, line);
	}
	if (line && ft_strncmp(line, "EA", 2) == 0)
	{
		set_texture(&game->east, line);
	}
	return (NULL);
}

void	init_textures(t_game *game)
{
	char	*line;

	game->fd_map = open(game->map_path, O_RDWR);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	while (line)
	{
		while (line && line[0] && !ft_istexture(line[0]))
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		printf("hit: %s\n", line);
		split_textures(game, line);
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}
