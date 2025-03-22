/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_animation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:37:35 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/22 18:57:38 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	update_menu_animations(t_game *g)
{
	double	current_time;

	current_time = get_time_ms();
	if (g->menu->bg.active && g->menu->bg.frame_count > 1)
	{
		if (current_time
			- g->menu->bg.last_update >= g->menu->bg.frame_duration)
		{
			g->menu->bg.current_frame = (g->menu->bg.current_frame + 1)
				% g->menu->bg.frame_count;
			g->menu->bg.last_update = current_time;
		}
	}
	update_cube_exit_animations(g, current_time);
}

void	update_cube_exit_animations(t_game *g, double current_time)
{
	if (g->menu->cube.active && g->menu->cube.frame_count > 1)
	{
		if (current_time
			- g->menu->cube.last_update >= g->menu->cube.frame_duration)
		{
			g->menu->cube.current_frame = (g->menu->cube.current_frame + 1)
				% g->menu->cube.frame_count;
			g->menu->cube.last_update = current_time;
		}
	}
	update_start_exit_animations(g, current_time);
}

void	update_start_exit_animations(t_game *game, double current_time)
{
	if (game->menu->selected_option == 0)
	{
		update_button_animation(&game->menu->start_btn.anim, current_time);
		game->menu->exit_btn.anim.current_frame = 0;
	}
	else
	{
		update_button_animation(&game->menu->exit_btn.anim, current_time);
		game->menu->start_btn.anim.current_frame = 0;
	}
}

void	update_button_animation(t_animation *anim, double current_time)
{
	if (anim->active && anim->frame_count > 1)
	{
		if (current_time - anim->last_update >= anim->frame_duration)
		{
			anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
			anim->last_update = current_time;
		}
	}
}
