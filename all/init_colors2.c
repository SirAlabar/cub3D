/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_colors2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:03:17 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/11 19:06:16 by marsoare         ###   ########.fr       */
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
