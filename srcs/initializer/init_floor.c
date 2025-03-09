/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seu-nome <seu-email@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 10:00:00 by seu-nome          #+#    #+#             */
/*   Updated: 2025/03/03 10:00:00 by seu-nome         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static char	*get_random_floor(void)
{
	int	random_num;

	random_num = rand() % 10 + 1;
	if (random_num == 1)
		return (ft_strdup(FLOOR1));
	else if (random_num == 2)
		return (ft_strdup(FLOOR2));
	else if (random_num == 3)
		return (ft_strdup(FLOOR3));
	else if (random_num == 4)
		return (ft_strdup(FLOOR4));
	else if (random_num == 5)
		return (ft_strdup(FLOOR5));
	else if (random_num == 6)
		return (ft_strdup(FLOOR6));
	else if (random_num == 7)
		return (ft_strdup(FLOOR7));
	else if (random_num == 8)
		return (ft_strdup(FLOOR8));
	else if (random_num == 9)
		return (ft_strdup(FLOOR9));
	else
		return (ft_strdup(FLOOR10));
}

int	is_floor_line(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (ft_strncmp(&line[i], "FLOOR", 5) == 0)
	{
		return (1);
	}
	return (0);
}

void	process_floor_config(t_game *game, char *line)
{
	int		i;
	char	*path;

	if (!line)
		return ;
	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	i += 5;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] && line[i] != '\n')
	{
		path = ft_strtrim(&line[i], " \n\t\r");
		if (!path)
			return ;
	}
	else
	{
		path = get_random_floor();
		if (!path)
			return ;
	}
	game->has_floor = 1;
	game->floor_path = path;
}

void	init_floor(t_game *game)
{
	t_texture	*floor_tex;

	if (!game->has_floor)
	{
		return ;
	}
	floor_tex = texture_create(game, game->floor_path);
	if (!floor_tex)
	{
		game->floor.img = NULL;
		return ;
	}
	game->floor = *floor_tex;
	free(floor_tex);
	free(game->floor_path);
	game->floor_path = NULL;
}
