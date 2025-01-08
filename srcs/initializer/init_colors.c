/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_colors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:10:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/08 18:39:23 by marsoare         ###   ########.fr       */
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
		{
			printf("found: %s\n", line);
			rgb_to_hex(line);
		}
		else if (line && line[0] == 'C')
		{
			printf("found: %s\n", line);
			rgb_to_hex(line);
		}
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}

//Double index approach
int	rgb_to_hex(char *color)
{
	int	i;
	int	j;
	char	*subs;

	i = 0;
	while (!ft_isdigit(color[i])) //jump til a number start
		i++;
	printf("start: %c\n", color[i]);
	j = i;
	while (color[j + 1] != ',') //jump til a number end
	{
		printf("%c\n", color[j]);
		j++;
	}
	printf("end: %c\n", color[i]);
	subs = ft_substr(color, i, j - i + 1);
	printf("subs: %s\n", subs);
	return (i);
}
