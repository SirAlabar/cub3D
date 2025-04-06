/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 19:45:38 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/12 14:20:40 by marsoare         ###   ########.fr       */
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

void	validate_color(char *line, int *color_count)
{
	int	i;

	i = 0;
	while (line && line[i] && ft_isspace(line[i]))
		i++;
	if (line && line[i])
	{
		if ((line[i] == 'F' || line[i] == 'C') && (line[i + 1] == ' '
				|| ft_isspace(line[i + 1])))
			(*color_count)++;
	}
	free(line);
}

bool	check_colors(t_game *game)
{
	char	*line;
	int		color_count;

	color_count = 0;
	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		return (read_error(game), exit(1), false);
	line = get_next_line(game->fd_map);
	while (line)
	{
		validate_color(line, &color_count);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
	if (color_count != 2)
		return (ft_putendl_fd("Error\ncolors", 2), false);
	return (true);
}

bool	checker(t_game *game, char *path)
{
	game->map_path = path;
	if (!check_colors(game))
		return (cleanup_game(game), false);
	return (true);
}
