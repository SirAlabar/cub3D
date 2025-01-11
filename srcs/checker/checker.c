/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:45:38 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/07 12:08:39 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	ft_iscolor(char c)
{
	int		i;
	char	*arr;

	i = 0;
	arr = "FC";
	while (arr[i])
	{
		if (c == arr[i])
			return (true);
		i++;
	}
	return (false);
}

int	count_color(int flag)
{
	static int count = 0;
	if (flag)
		count++;
	return (count);
}

bool	check_colors(t_game *game)
{
	char	*line;
	int		i;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	i = 0;
	while (line)
	{
		while(line && line[i] && ft_isspace(line[i]))
			i++;
		while (line && line[i] && !ft_iscolor(line[i]))
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		if (line && line[i] && ft_iscolor(line[i]))
			count_color(1);
		free(line);
		line = get_next_line(game->fd_map);
		i = 0;
	}
	close(game->fd_map);
	if (count_color(0) != 2)
		return (ft_putendl_fd("Error\ncolors", 2), false);
	return (true);
}

bool	checker(t_game *game, char *path)
{
	(void) path;
	game->map_path = path;
	if (!check_colors(game))
		return (cleanup_game(game), false);
	return (true);
}
