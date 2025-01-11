/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_colors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:10:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/09 19:07:04 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_color_component(char *color, int *start)
{
	int		j;
	char	*subs;
	int		value;

	while (color[*start] && ft_isspace(color[*start]))
		(*start)++;
	j = *start;
	while (color[j] && color[j] != ',' && color[j] != '\n' && !ft_isspace(color[j]))
		j++;
	subs = ft_substr(color, *start, j - *start);
	value = ft_atoi(subs);
	free(subs);
	while (color[j] && (ft_isspace(color[j]) || color[j] == ','))
		j++;
	*start = j;
	return (value);
}

int	rgb_to_hex(char *color)
{
	int	i;
	int	r;
	int	g;
	int	b;

	i = 0;
	while (!ft_isdigit(color[i]))
		i++;
	r = parse_color_component(color, &i);
	g = parse_color_component(color, &i);
	b = parse_color_component(color, &i);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return (((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff));
}

int	get_number(int flag)
{
	static int	a = 0;
	if (flag == 42)
		a = 1;
	else if (flag == -1)
		return (a);
	return (0);
}

void	assign_color(t_game *game, char *line)
{
	int	color;

	if (line[0] == 'F')
	{
		color = rgb_to_hex(line);
		if (color == -1)
		{
			ft_putendl_fd("Error\nInvalid floor color value (must be 0-255)", 2);
			get_number(42);
		}
		game->map.floor_color = color;
	}
	else if (line[0] == 'C')
	{
		color = rgb_to_hex(line);
		if (color == -1)
		{
			ft_putendl_fd("Error\nInvalid ceiling color value (must be 0-255)", 2);
			get_number(42);
		}
		game->map.ceiling_color = color;
	}
}

void	norm_norm(t_game *game, char *line, char **c, char **f)
{
	if (line && line[0] == 'F')
	{
		free(*f);
		*f = line;
		assign_color(game, line);
	}
	else if (line && line[0] == 'C')
	{
		free(*c);
		*c = line;
		assign_color(game, line);
	}
}

void	init_colors(t_game *game)
{
	char	*line;
	char	*floor_line = NULL;
	char	*ceiling_line = NULL;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		return (read_error(game));
	line = get_next_line(game->fd_map);
	while (line)
	{
		while (line && line[0] != 'F' && line[0] != 'C')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		norm_norm(game, line, &ceiling_line, &floor_line);
		line = get_next_line(game->fd_map);
	}
	free(floor_line);
	free(ceiling_line);
	close(game->fd_map);
	printf(RED"%i\n"DEFAULT, get_number(-1));
	if (get_number(-1) > 0)
	{
		cleanup_game(game);
		exit(1);
	}
}
