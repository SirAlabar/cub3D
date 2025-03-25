/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:02:16 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/23 14:02:27 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_texture	*load_enemy_texture_original(t_game *game)
{
	t_texture	*texture;
	char		*path;

	path = "./assets/sprites/enemies/cacodemon00.xpm";
	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf(RED"Error\nFailed to load enemy texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

void	add_enemy(t_game *game, t_vector pos)
{
	t_enemy_list	*new;
	t_enemy_list	*temp;

	new = (t_enemy_list *)ft_calloc(sizeof(t_enemy_list), 1);
	if (!new)
		return ;
	new->enemy.pos = pos;
	new->enemy.health = 3;
	new->enemy.alive = true;
	new->enemy.dir = vector_create(0, 0);
	new->enemy.texture = load_enemy_texture_original(game);
	new->enemy.detection_radius = 5.0;
	new->enemy.last_attack = 0.0;
	new->enemy.current_frame = 0;
	new->enemy.last_frame_change = get_time_ms();
	new->next = NULL;
	if (!game->enemies)
		game->enemies = new;
	else
	{
		temp = game->enemies;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
