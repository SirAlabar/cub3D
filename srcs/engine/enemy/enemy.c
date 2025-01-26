/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:40:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/25 10:21:45 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	vector_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

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
	new->enemy.health = 100;
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

bool	can_enemy_move_x(t_game *game, t_vector new_position, t_vector direction, double padding)
{
	int	map_x;
	int	map_y;

	map_x = (int)(new_position.x + direction.x * padding);
	map_y = (int)(new_position.y);
	if (map_x >= 0 && map_x < game->map.width && map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}

bool	can_enemy_move_y(t_game *game, t_vector new_position, t_vector direction, double padding)
{
	int	map_x;
	int	map_y;

	map_x = (int)(new_position.x);
	map_y = (int)(new_position.y + direction.y * padding);
	if (map_x >= 0 && map_x < game->map.width && map_y >= 0 && map_y < game->map.height)
	{
		if (game->map.grid[map_y][map_x] == '0')
			return (true);
	}
	return (false);
}

void	calculate_enemy_distance(t_game *game, t_enemy *enemy)
{
	t_vector	dir_vector;
	double		length;

	dir_vector = vector_sub(game->p1.pos, enemy->pos);
	length = vector_length(dir_vector);
	if (length > 0.0001)
		enemy->dir = vector_div(dir_vector, length);
	else
		enemy->dir = vector_create(0, 0);
	enemy->dist_to_player = length;
}

void	update_enemy_position(t_enemy *enemy, t_game *game, double speed)
{
	t_vector	movement;
	t_vector	new_position;

	movement = vector_mult(enemy->dir, speed);
	new_position = enemy->pos;
	new_position.x += movement.x;
	if (can_enemy_move_x(game, new_position, enemy->dir, 0.5))
		enemy->pos.x = new_position.x;
	new_position = enemy->pos;
	new_position.y += movement.y;
	if (can_enemy_move_y(game, new_position, enemy->dir, 0.5))
		enemy->pos.y = new_position.y;
}

void	update_enemies(t_game *game)
{
	t_enemy_list	*current;
	double			speed;

	current = game->enemies;
	speed = 0.03;
	while (current != NULL)
	{
		if (current->enemy.alive)
		{
			calculate_enemy_distance(game, &current->enemy);
			if (current->enemy.dist_to_player <= current->enemy.detection_radius)
				update_enemy_position(&current->enemy, game, speed);
		}
		current = current->next;
	}
}
