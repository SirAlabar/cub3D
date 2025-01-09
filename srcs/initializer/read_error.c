/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:19:43 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/09 18:23:32 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	read_error(t_game *game)
{
	ft_putstr_fd(ERROR_OPEN, 2);
	cleanup_game(game);
}
