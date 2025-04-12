/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_colors2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:03:17 by marsoare          #+#    #+#             */
/*   Updated: 2025/04/12 15:30:18 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	get_number(int flag)
{
	static int	a = 0;

	if (flag == 42)
		a = 1;
	else if (flag == -1)
		return (a);
	return (0);
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

void	handle_color_error(t_game *game, char *message)
{
	ft_putendl_fd(message, 2);
	cleanup_game(game);
	exit(1);
}
