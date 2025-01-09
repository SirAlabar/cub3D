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

#include <cub3d.h>

void	init_colors(t_game *game)
{
	char	*line;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	while (line)
	{
		while (line && line[0] != 'F' && line[0] != 'C')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		if (line && line[0] == 'F')
		{
			game->map.floor_color = rgb_to_hex(line);
		}
		else if (line && line[0] == 'C')
		{
			game->map.ceiling_color = rgb_to_hex(line);
		}
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}

unsigned int	create_rgb(int r, int g, int b)
{
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return (((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff));
}

int	parse_color_component(char *color, int *start)
{
	int		j;
	char	*subs;
	int		value;

	j = *start;
	while (color[j] && color[j] != ',' && color[j] != '\n' && color[j] != ' ')
		j++;
	subs = ft_substr(color, *start, j - *start);
	value = ft_atoi(subs);
	free(subs);
	*start = j + 1;
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
	return (create_rgb(r, g, b));
}
