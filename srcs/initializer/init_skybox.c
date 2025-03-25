/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:50:16 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:50:18 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_skybox(t_game *game)
{
	t_texture	*skybox_tex;

	if (!game->has_skybox)
	{
		return ;
	}
	skybox_tex = texture_create(game, game->skybox_path);
	if (!skybox_tex)
	{
		ft_printf("Warning: Failed to load skybox texture: %s\n",
			game->skybox_path);
		game->skybox.img = NULL;
		return ;
	}
	game->skybox = *skybox_tex;
	free(skybox_tex);
	free(game->skybox_path);
	game->skybox_path = NULL;
}

int	is_skybox_line(char *line)
{
	if (!line)
		return (0);
	while (*line && ft_isspace(*line))
		line++;
	if (ft_strncmp(line, "SKYBOX", 6) == 0)
	{
		return (1);
	}
	return (0);
}

static char	*get_random_skybox(void)
{
	int	random_num;

	random_num = rand() % 10 + 1;
	if (random_num == 1)
		return (ft_strdup(SKYBOX1));
	else if (random_num == 2)
		return (ft_strdup(SKYBOX2));
	else if (random_num == 3)
		return (ft_strdup(SKYBOX3));
	else if (random_num == 4)
		return (ft_strdup(SKYBOX4));
	else if (random_num == 5)
		return (ft_strdup(SKYBOX5));
	else if (random_num == 6)
		return (ft_strdup(SKYBOX6));
	else if (random_num == 7)
		return (ft_strdup(SKYBOX7));
	else if (random_num == 8)
		return (ft_strdup(SKYBOX8));
	else if (random_num == 9)
		return (ft_strdup(SKYBOX9));
	else
		return (ft_strdup(SKYBOX10));
}

void	process_skybox_config(t_game *game, char *line)
{
	int		i;
	char	*path;

	if (!line)
		return ;
	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	i += 6;
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
		path = get_random_skybox();
		if (!path)
			return ;
	}
	game->has_skybox = 1;
	game->skybox_path = path;
}
