/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:21:10 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/07 11:26:25 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	print_map(t_game *game)
{
	int	i;

	i = 0;
	while (game->map.grid[i])
	{
		printf("%s", game->map.grid[i]);
		i++;
	}
	printf("---------------------\n");
	printf("Map height: %i\n", game->map.height);
	printf("Map width: %i\n", game->map.width);
}
