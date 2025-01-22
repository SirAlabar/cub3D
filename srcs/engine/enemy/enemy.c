/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:00:00 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/22 18:26:32 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_texture    *load_enemy_texture(t_game *game)
{
	t_texture    *texture;
	char        *path;

	path = "./assets/sprites/enemies/cacodemon00.xpm";
	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf("Error: Failed to load enemy texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

void    add_enemy(t_game *game, t_vector pos)
{
	t_enemy_list    *new;
	t_enemy_list    *temp;

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

static void calculate_enemy_distance(t_game *game, t_enemy *enemy)
{
	enemy->dist_to_player = vector_dist(game->p1.pos, enemy->pos);
	enemy->dir = vector_normalize(vector_sub(game->p1.pos, enemy->pos));
}

void    update_enemies(t_game *game)
{
	t_enemy_list    *current;

	current = game->enemies;
	while (current)
	{
		if (current->enemy.alive)
			calculate_enemy_distance(game, &current->enemy);
		current = current->next;
	}
}

void    draw_enemies(t_game *game)
{
	t_enemy_list    *current;
	double          angle;
	double          sprite_x;
	int             sprite_height;
	int             sprite_width;

	current = game->enemies;
	while (current)
	{
	printf("OMG LETS SEE drawn here\n");
		if (!current->enemy.alive)
		{
			current = current->next;
			continue;
		}
		angle = atan2(current->enemy.dir.y, current->enemy.dir.x) - 
			atan2(game->p1.dir.y, game->p1.dir.x);
		sprite_x = (WINDOW_WIDTH / 2) * (1 + angle / (M_PI / 3));
		sprite_height = WINDOW_HEIGHT / current->enemy.dist_to_player;
		sprite_width = sprite_height;
		if (sprite_height > WINDOW_HEIGHT)
			sprite_height = WINDOW_HEIGHT;
		current = current->next;
	}
}

void    init_enemies(t_game *game)
{
	game->enemies = NULL;
	// Add initial enemies
	add_enemy(game, vector_create(1.0, 1.0));
	add_enemy(game, vector_create(2.0, 2.0));
}
