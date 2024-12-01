/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:15:56 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:16:21 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	validate_map(t_game *game, char **argv)
{
	(void) game;
	printf("checking map: %s\n", argv[1]);
	if (!check_extension(argv[1]))
		return (printf("message\n"), false);
	return (true);
}
