/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 22:00:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	texture_destroy(t_texture **texture, void *mlx)
{
	if (*texture)
	{
		if ((*texture)->img)
		{
			mlx_destroy_image(mlx, (*texture)->img);
			(*texture)->img = NULL;
		}
		free(*texture);
		*texture = NULL;
	}
}

void	destroy_sprite(t_game *game, t_sprite *sprite)
{
	int	i;

	if (!sprite)
		return ;
	i = 0;
	while (i < MAX_FRAMES)
	{
		if (sprite->frames[i])
		{
			texture_destroy(&sprite->frames[i], game->mlx);
			sprite->frames[i] = NULL;
		}
		i++;
	}
	sprite->current_frame = 0;
	sprite->is_animated = false;
	sprite->is_solid = false;
}

void	cleanup_sprites(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = 0;
	while (i < game->num_sprites)
	{
		destroy_sprite(game, &game->sprites[i]);
		i++;
	}
}
