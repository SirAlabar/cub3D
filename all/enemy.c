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

void	calculate_enemy_distance(t_game *game, t_enemy *enemy)
{
    enemy->dist_to_player = vector_dist(game->p1.pos, enemy->pos);
    t_vector dir_vector = vector_sub(game->p1.pos, enemy->pos);
    enemy->dir = vector_normalize(dir_vector);
}

void	update_enemies(t_game *game)
{
    t_enemy_list	*current = game->enemies;
    double			speed = 0.03; // Adjust speed as needed

    while (current)
    {
        if (current->enemy.alive)
        {
            calculate_enemy_distance(game, &current->enemy);
            t_vector movement = vector_mult(current->enemy.dir, speed);

            // Attempt X movement
            t_vector new_x_pos = current->enemy.pos;
            new_x_pos.x += movement.x;
            if (can_move_x(game, new_x_pos, current->enemy.dir, 0.5))
                current->enemy.pos.x = new_x_pos.x;

            // Attempt Y movement
            t_vector new_y_pos = current->enemy.pos;
            new_y_pos.y += movement.y;
            if (can_move_y(game, new_y_pos, current->enemy.dir, 0.5))
                current->enemy.pos.y = new_y_pos.y;
        }
        current = current->next;
    }
}
