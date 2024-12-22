/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:15:56 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/22 13:57:31 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	validate_map_extension(int argc, char **argv)
{
	if (argc != 2)
		return (printf(ERROR_USAGE), false);
	if (!valid_extension(argv[1]))
		return (printf(ERROR_USAGE ERROR_MAPTYPE), false);
	return (true);
}

bool	valid_extension(char *map)
{
	const char	*dot = ft_strrchr(map, '.');

	if (!dot)
		return (false);
	if (ft_strcmp((char *)dot, ".cub") == 0)
		return (true);
	return (false);
}
