/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:21:26 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	main(int argc, char **argv)
{
	t_game	*game;

	game = NULL;
	ft_bzero(&game, sizeof(t_game *));
	if (argc != 2)
		return (printf("Provide a map\n"), 1);// <- add custom message later
	if (!validate_map(game, argv))
		return (1);
	return (0);
}
