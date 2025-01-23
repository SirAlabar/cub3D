/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:40:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/22 19:47:36 by marsoare         ###   ########.fr       */
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
    // Keep facing a fixed direction
    enemy->dir = vector_create(1, 0);
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

static void draw_enemy_sprite(t_game *game, t_enemy *enemy, double screen_x, int sprite_height)
{
    int draw_start_y = WINDOW_HEIGHT / 2 - sprite_height / 2;
    int draw_end_y = WINDOW_HEIGHT / 2 + sprite_height / 2;
    int sprite_width = sprite_height;
    int draw_start_x = screen_x - sprite_width / 2;
    int draw_end_x = screen_x + sprite_width / 2;

    if (draw_start_y < 0) draw_start_y = 0;
    if (draw_end_y >= WINDOW_HEIGHT) draw_end_y = WINDOW_HEIGHT - 1;
    if (draw_start_x < 0) draw_start_x = 0;
    if (draw_end_x >= WINDOW_WIDTH) draw_end_x = WINDOW_WIDTH - 1;

    for (int stripe = draw_start_x; stripe < draw_end_x; stripe++)
    {
        int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + screen_x)) 
            * enemy->texture->width / sprite_width) / 256;

        if (tex_x >= 0 && tex_x < enemy->texture->width)
        {
            for (int y = draw_start_y; y < draw_end_y; y++)
            {
                int d = (y) * 256 - WINDOW_HEIGHT * 128 + sprite_height * 128;
                int tex_y = ((d * enemy->texture->height) / sprite_height) / 256;

                if (tex_y >= 0 && tex_y < enemy->texture->height)
                {
                    int color = get_texture_pixel(enemy->texture, tex_x, tex_y);
                    if ((color & 0x00FFFFFF) != 0)
                    {
                        double shade = 1.0 / (1.0 + enemy->dist_to_player * 0.04);
                        color = apply_shade(color, shade);
                        draw_pixel(game, stripe, y, color);
                    }
                }
            }
        }
    }
}

double vector_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

static bool is_enemy_visible(t_game *game, t_vector enemy_pos)
{
    t_vector ray_dir = vector_normalize(vector_sub(enemy_pos, game->p1.pos));
    t_vector ray_pos = game->p1.pos;
    t_vector delta_dist;
    t_vector side_dist;
    t_vector step;
    int map_x = (int)ray_pos.x;
    int map_y = (int)ray_pos.y;

    delta_dist.x = fabs(1 / ray_dir.x + 0.3);
    delta_dist.y = fabs(1 / ray_dir.y);

    if (ray_dir.x < 0)
    {
        step.x = -1;
        side_dist.x = (ray_pos.x - map_x) * delta_dist.x;
    }
    else
    {
        step.x = 1;
        side_dist.x = (map_x + 1.0 - ray_pos.x) * delta_dist.x;
    }

    if (ray_dir.y < 0)
    {
        step.y = -1;
        side_dist.y = (ray_pos.y - map_y) * delta_dist.y;
    }
    else
    {
        step.y = 1;
        side_dist.y = (map_y + 1.0 - ray_pos.y) * delta_dist.y;
    }

    while (1)
    {
        if (side_dist.x < side_dist.y)
        {
            side_dist.x += delta_dist.x;
            map_x += step.x;
        }
        else
        {
            side_dist.y += delta_dist.y;
            map_y += step.y;
        }

        // Check if the ray has reached the enemy's position
        if (map_x == (int)enemy_pos.x && map_y == (int)enemy_pos.y)
            return true; // Enemy is visible

        // Check if the ray has hit a wall
        if (game->map.grid[map_x][map_y] == '1')
            return false; // Wall blocks the view
    }
}
void draw_enemies(t_game *game)
{
    t_enemy_list *current = game->enemies;

    // Calculate the FOV dynamically using the player's plane vector
    double plane_length = vector_length(game->p1.plane);
    double fov = 2 * atan2(plane_length, 1.0);

    while (current)
    {
        if (!current->enemy.alive)
        {
            current = current->next;
            continue;
        }

        // Check if the enemy is visible (not blocked by a wall)
        if (!is_enemy_visible(game, current->enemy.pos))
        {
            current = current->next;
            continue;
        }

        // Calculate the relative position of the enemy to the player
        t_vector relative_pos = vector_sub(current->enemy.pos, game->p1.pos);

        // Calculate the distance to the enemy
        double dist_to_enemy = vector_dist(game->p1.pos, current->enemy.pos);

        // Calculate the angle to the enemy relative to the player's view direction
        double angle_to_enemy = atan2(relative_pos.y, relative_pos.x) - atan2(game->p1.dir.y, game->p1.dir.x);

        // Normalize the angle to be within the field of view
        if (angle_to_enemy < -M_PI)
            angle_to_enemy += 2 * M_PI;
        if (angle_to_enemy > M_PI)
            angle_to_enemy -= 2 * M_PI;

        // Check if the enemy is within the player's field of view
        if (fabs(angle_to_enemy) < fov / 2)
        {
            // Calculate the screen position of the enemy
            double screen_x = (angle_to_enemy + fov / 2) * (WINDOW_WIDTH / fov);

            // Calculate the height of the enemy on the screen
            int sprite_height = (int)(WINDOW_HEIGHT / dist_to_enemy);

            // Draw the enemy sprite
            draw_enemy_sprite(game, &current->enemy, screen_x, sprite_height);
        }

        current = current->next;
    }
}
static void spawn_enemies_from_map(t_game *game)
{
    for (int i = 0; i < game->map.height; i++)
    {
        for (int j = 0; j < game->map.width; j++)
        {
            if (game->map.grid[i][j] == 'E')
            {
                add_enemy(game, vector_create(j + 0.5, i + 0.5)); // Center the enemy in the grid cell
                game->map.grid[i][j] = '0'; // Replace 'E' with '0' to avoid respawning
            }
        }
    }
}

void init_enemies(t_game *game)
{
	game->enemies = NULL;
	spawn_enemies_from_map(game);
}
