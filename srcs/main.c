/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:50:11 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	ft_bzero(&game, sizeof(t_game *));
	if (!validate_map_extension(argc, argv))
		return (1);
	return (0);
}
