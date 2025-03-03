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


void	init_skybox(t_game *game)
{
	t_texture	*skybox_tex;
	// if (!game->has_skybox)
	// 	return ;
	skybox_tex = texture_create(game, SKYBOX10);
	if (!skybox_tex)
	{
		// ft_printf("Warning: Failed to load skybox texture: %s\n", game->skybox_path);
		game->skybox.img = NULL;
		return ;
	}
	
	game->skybox = *skybox_tex;
	free(skybox_tex);
	// free(game->skybox_path);
	// game->skybox_path = NULL;
	// ft_printf("Skybox loaded: %dx%d pixels\n", game->skybox.width, game->skybox.height);
}

// int	is_skybox_line(char *line)
// {
// 	if (!line)
// 		return (0);
// 	while (*line && ft_isspace(*line))
// 		line++;
// 	if (ft_strncmp(line, "SKYBOX", 6) == 0)
// 		return (1);
// 	return (0);
// }

// static char	*get_random_skybox(void)
// {
// 	int	random_num;
	
// 	random_num = rand() % 10 + 1;
// 	if (random_num == 1)
// 		return (ft_strdup(SKYBOX1));
// 	else if (random_num == 2)
// 		return (ft_strdup(SKYBOX2));
// 	else if (random_num == 3)
// 		return (ft_strdup(SKYBOX3));
// 	else if (random_num == 4)
// 		return (ft_strdup(SKYBOX4));
// 	else if (random_num == 5)
// 		return (ft_strdup(SKYBOX5));
// 	else if (random_num == 6)
// 		return (ft_strdup(SKYBOX6));
// 	else if (random_num == 7)
// 		return (ft_strdup(SKYBOX7));
// 	else if (random_num == 8)
// 		return (ft_strdup(SKYBOX8));
// 	else if (random_num == 9)
// 		return (ft_strdup(SKYBOX9));
// 	else
// 		return (ft_strdup(SKYBOX10));
// }

// void	process_skybox_config(t_game *game, char *line)
// {
// 	int		i;
// 	char	*path;
	
// 	if (!line)
// 		return ;
// 	i = 0;
// 	while (line[i] && ft_isspace(line[i]))
// 		i++;
// 	i += 6;
// 	while (line[i] && ft_isspace(line[i]))
// 		i++;
// 	if (line[i] && line[i] != '\n')
// 	{
// 		path = ft_strtrim(&line[i], " \n\t\r");
// 		if (!path)
// 			return ;
// 	}
// 	else
// 	{
// 		path = get_random_skybox();
// 		if (!path)
// 			return ;
// 	}
// 	// game->has_skybox = 1;
// 	// game->skybox_path = path;
// }

// void	init_floor(t_game *game)
// {
// 	t_texture	*floor_tex;

// 	floor_tex = texture_create(game, "assets/textures/floor.xpm");
// 	if (!floor_tex)
// 	{
// 		ft_printf("Warning: Failed to load floor texture\n");
// 		game->floor.img = NULL;
// 		return ;
// 	}
// 	game->floor = *floor_tex;
// 	free(floor_tex);
// }
