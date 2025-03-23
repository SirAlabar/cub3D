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

/**
 * Loads a specific enemy texture frame
 * @param game Game instance
 * @param enemy Enemy to load texture for
 * @param frame Frame number (0-3)
 */
void	load_enemy_texture(t_game *game, t_enemy *enemy, int frame)
{
	char	*path;

	path = ft_calloc(50, sizeof(char));
	if (!path)
		return ;
	sprintf(path, "./assets/sprites/enemies/cacodemon%02d.xpm", frame);
	
	if (enemy->texture)
		texture_destroy(&enemy->texture, game->mlx);
	
	enemy->texture = texture_create(game, path);
	if (!enemy->texture)
		ft_printf("Error\nFailed to load enemy texture: %s\n", path);
	
	free(path);
}

/**
 * Updates the current animation frame based on time
 * @param enemy Enemy to update
 * @param game Game instance for texture loading
 */
void	update_enemy_animation(t_enemy *enemy, t_game *game)
{
	double	current_time;
	
	if (!enemy || !enemy->alive)
		return ;
	
	current_time = get_time_ms();
	
	// Only update animation if enough time has passed
	if (current_time - enemy->last_frame_change > ENEMY_ANIM_SPEED)
	{
		enemy->current_frame = (enemy->current_frame + 1) % NUM_ENEMY_FRAMES;
		enemy->last_frame_change = current_time;
		
		// Load the new texture frame
		load_enemy_texture(game, enemy, enemy->current_frame);
	}
}

/**
 * Updates animations for all enemies
 * @param game Game instance
 */
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
