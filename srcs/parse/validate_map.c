/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:15:56 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:36:51 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	validate_map(t_game *game, int argc, char **argv)
{
	(void) game;
	if (argc != 2)
		return (printf(ERROR_USAGE), 1);// <- add custom message later
	if (!valid_extension(argv[1]))
		return (printf(ERROR_USAGE ERROR_MAPTYPE), false);
	return (true);
}

bool	valid_extension(char *map)
{
	const char	*dot = ft_strchr(map, '.');

	if (!dot)
		return (false);
	if (ft_strcmp((char *)dot, ".cub") == 0)
		return (true);
	return (false);
}
