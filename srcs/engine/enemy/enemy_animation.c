/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 18:00:00 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/23 18:30:00 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	load_enemy_texture(t_game *game, t_enemy *enemy, char *path)
{
	if (!path)
		return ;
	
	if (enemy->texture)
		texture_destroy(&enemy->texture, game->mlx);
	
	enemy->texture = texture_create(game, path);
	if (!enemy->texture)
		ft_printf("Error\nFailed to load enemy texture: %s\n", path);
}

void	update_enemy_animation(t_enemy *enemy, t_game *game)
{
	double	current_time;
	char	*path;

	if (!enemy || !enemy->alive)
		return ;
	
	current_time = get_time_ms();
	if (current_time - enemy->last_frame_change > ENEMY_ANIM_SPEED)
	{
		enemy->current_frame = (enemy->current_frame + 1) % NUM_ENEMY_FRAMES;
		enemy->last_frame_change = current_time;
		
		// Allocate and build the path here
		path = ft_calloc(50, sizeof(char));
		if (!path)
			return ;
		
		// Format the path with the current frame
		snprintf(path, 50, "./assets/sprites/enemies/cacodemon%02d.xpm", 
			enemy->current_frame);
		
		// Pass the path to the load function
		load_enemy_texture(game, enemy, path);
		
		// Free the allocated memory
		free(path);
	}
}

void	update_enemy_animations(t_game *game)
{
	t_enemy_list	*current;

	if (!game || !game->enemies)
		return ;
	current = game->enemies;
	while (current != NULL)
	{
		if (current->enemy.alive)
			update_enemy_animation(&current->enemy, game);
		current = current->next;
	}
}
