
#include <cub3d.h>

void	update_menu_animations(t_game *game)
{
	double	current_time;

	current_time = get_time_ms();
	if (game->menu->background.active && game->menu->background.frame_count > 1)
	{
		if (current_time - game->menu->background.last_update
			>= game->menu->background.frame_duration)
		{
			game->menu->background.current_frame
				= (game->menu->background.current_frame + 1)
				% game->menu->background.frame_count;
			game->menu->background.last_update = current_time;
		}
	}
	update_cube_exit_animations(game, current_time);
}

void	update_cube_exit_animations(t_game *game, double current_time)
{
	if (game->menu->cube.active && game->menu->cube.frame_count > 1)
	{
		if (current_time - game->menu->cube.last_update
			>= game->menu->cube.frame_duration)
		{
			game->menu->cube.current_frame = (game->menu->cube.current_frame + 1)
				% game->menu->cube.frame_count;
			game->menu->cube.last_update = current_time;
		}
	}
	update_start_exit_animations(game, current_time);
}

void	update_start_exit_animations(t_game *game, double current_time)
{
	if (game->menu->start_btn.selected)
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