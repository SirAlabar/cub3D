/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:11:37 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:11:54 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	handle_firing(t_game *game)
{
	game->p1.is_firing = 1;
	shoot_enemy(game);
	if (game->sounds && game->sounds->is_initialized)
		play_sound(game->sounds->gun);
}
