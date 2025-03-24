/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:18:58 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:48:58 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0);
}

void	update_fps(t_game *game)
{
	static double	last_time = 0;
	static int		frame_count = 0;
	double			current_time;

	current_time = get_time_ms();
	if (last_time == 0)
		last_time = current_time;
	frame_count++;
	if (current_time - last_time >= 1000.0)
	{
		game->fps = frame_count * 1000.0 / (current_time - last_time);
		frame_count = 0;
		last_time = current_time;
	}
}
