/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_menu.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:53:54 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:49:05 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	cleanup_animation(t_game *game, t_animation *anim)
{
	int	i;

	i = 0;
	while (i < anim->frame_count && i < MAX_FRAMES)
	{
		if (anim->frames[i].img)
		{
			mlx_destroy_image(game->mlx, anim->frames[i].img);
			anim->frames[i].img = NULL;
			anim->frames[i].addr = NULL;
		}
		i++;
	}
	anim->frame_count = 0;
	anim->active = 0;
}

static void	cleanup_menu_item(t_game *game, t_menu_item *item)
{
	if (!game || !item)
		return ;
	cleanup_animation(game, &item->anim);
	item->selected = 0;
}

void	cleanup_menu(t_game *game)
{
	if (!game || !game->menu)
		return ;
	cleanup_animation(game, &game->menu->bg);
	cleanup_animation(game, &game->menu->cube);
	cleanup_animation(game, &game->menu->select_gun);
	cleanup_menu_item(game, &game->menu->start_btn);
	cleanup_menu_item(game, &game->menu->exit_btn);
	game->menu->active = 0;
	game->menu->selected_option = 0;
	free(game->menu);
	game->menu = NULL;
}
