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
		printf("ENEMY WAS ADD\n");
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

static void draw_enemy_sprite(t_game *game, t_enemy *enemy, 
    double sprite_x, int sprite_height)
{
    int draw_start_y = WINDOW_HEIGHT/2 - sprite_height/2;
    int draw_end_y = WINDOW_HEIGHT/2 + sprite_height/2;
    int sprite_width = sprite_height; // 1:1 aspect ratio
    int draw_start_x = sprite_x - sprite_width/2;
    int draw_end_x = sprite_x + sprite_width/2;
    
    // Screen bounds checking
    if (draw_start_y < 0) draw_start_y = 0;
    if (draw_end_y >= WINDOW_HEIGHT) draw_end_y = WINDOW_HEIGHT - 1;
    if (draw_start_x < 0) draw_start_x = 0;
    if (draw_end_x >= WINDOW_WIDTH) draw_end_x = WINDOW_WIDTH - 1;

    // Draw sprite
    for(int stripe = draw_start_x; stripe < draw_end_x; stripe++)
    {
        int tex_x = (int)(256 * (stripe - (-sprite_width/2 + sprite_x)) 
            * enemy->texture->width / sprite_width) / 256;
        
        if(tex_x >= 0 && tex_x < enemy->texture->width)
        {
            for(int y = draw_start_y; y < draw_end_y; y++)
            {
                int d = (y) * 256 - WINDOW_HEIGHT * 128 + sprite_height * 128;
                int tex_y = ((d * enemy->texture->height) / sprite_height) / 256;
                
                if(tex_y >= 0 && tex_y < enemy->texture->height)
                {
                    int color = get_texture_pixel(enemy->texture, tex_x, tex_y);
                    if((color & 0x00FFFFFF) != 0)
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

void draw_enemies(t_game *game)
{
    t_enemy_list *current = game->enemies;
    
    while (current)
    {
        if (!current->enemy.alive)
        {
            current = current->next;
            continue;
        }

        // Calculate angle between enemy and player's view direction
        double angle = atan2(current->enemy.dir.y, current->enemy.dir.x) - 
            atan2(game->p1.dir.y, game->p1.dir.x);
            
        // Keep angle in [-π,π]
        while (angle < -M_PI) angle += 2*M_PI;
        while (angle > M_PI) angle -= 2*M_PI;

        // Check if enemy is in field of view (FOV is 60 degrees = π/3 radians)
        if (fabs(angle) < M_PI/3)
        {
            double sprite_x = (WINDOW_WIDTH/2) * (1 + tan(angle));
            int sprite_height = (int)(WINDOW_HEIGHT / current->enemy.dist_to_player);
            
            if (sprite_height > 0 && sprite_x > 0 && sprite_x < WINDOW_WIDTH)
            {
                draw_enemy_sprite(game, &current->enemy, sprite_x, sprite_height);
            }
        }
        
        current = current->next;
    }
}
void    init_enemies(t_game *game)
{
	game->enemies = NULL;
	// Add initial enemies
	add_enemy(game, vector_create(1, 1));
	add_enemy(game, vector_create(2, 2));
}
