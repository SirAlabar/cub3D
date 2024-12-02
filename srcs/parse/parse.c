/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:22:02 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/12/01 18:12:07 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	game_parse(t_game *game, char *map_path)
{
	parse_map(game, map_path);
}

void	parse_map(t_game *game, char *map_path)
{
	int	map_fd;

	map_fd = open_map_file(map_path);
	set_map(game, map_fd);
}

void	set_map(t_game *game, int map_fd)
{
	(void) game;
	(void) map_fd;
}

int	open_map_file(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Error opening map file %s\n", file);
		return (-1);
	}
	return (fd);
}
