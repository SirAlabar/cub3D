/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:40:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/26 17:37:53 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_texture	*load_enemy_texture(t_game *game)
{
	t_texture	*texture;
	char		*path;

	path = "./assets/sprites/enemies/cacodemon00.xpm";
	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf("Error\nFailed to load enemy texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

void	add_enemy(t_game *game, t_vector pos)
{
	t_enemy_list	*new;
	t_enemy_list	*temp;

	new = (t_enemy_list *)malloc(sizeof(t_enemy_list));
	if (!new)
		return ;
	new->enemy.pos = pos;
	new->enemy.health = 3;
	new->enemy.alive = true;
	new->enemy.dir = vector_create(0, 0);
	new->enemy.texture = load_enemy_texture(game);
	new->enemy.detection_radius = 5.0;
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

bool	c_enemy_mx(t_game *game, t_vector n_pos, t_vector dir, double padd)
{
	int	map_x;
	int	map_y;

	map_x = (int)(n_pos.x + dir.x * padd);
	map_y = (int)(n_pos.y);
	if (map_x >= 0 && map_x < game->map.width
		&& map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}

bool	c_enemy_my(t_game *game, t_vector n_pos, t_vector dir, double padd)
{
	int	map_x;
	int	map_y;

	map_x = (int)(n_pos.x);
	map_y = (int)(n_pos.y + dir.y * padd);
	if (map_x >= 0 && map_x < game->map.width
		&& map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}
