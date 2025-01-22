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

static void draw_enemy_sprite(t_game *game, t_enemy *enemy, 
    double sprite_x, int sprite_height)
{
    int draw_start_y = WINDOW_HEIGHT/2 - sprite_height/2;
    int draw_end_y = WINDOW_HEIGHT/2 + sprite_height/2;
    int sprite_width = sprite_height;
    int draw_start_x = sprite_x - sprite_width/2;
    int draw_end_x = sprite_x + sprite_width/2;
    
    if (draw_start_y < 0) draw_start_y = 0;
    if (draw_end_y >= WINDOW_HEIGHT) draw_end_y = WINDOW_HEIGHT - 1;
    if (draw_start_x < 0) draw_start_x = 0;
    if (draw_end_x >= WINDOW_WIDTH) draw_end_x = WINDOW_WIDTH - 1;

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
    t_vector relative_pos;

    while (current)
    {
        if (!current->enemy.alive)
        {
            current = current->next;
            continue;
        }

        // Calculate relative position to player
        relative_pos.x = current->enemy.pos.x - game->p1.pos.x;
        relative_pos.y = current->enemy.pos.y - game->p1.pos.y;

        // Transform relative position by player's view direction
        double transformed_x = relative_pos.x * game->p1.dir.y - relative_pos.y * game->p1.dir.x;
        double transformed_y = relative_pos.x * game->p1.dir.x + relative_pos.y * game->p1.dir.y;

        if (transformed_y > 0.1) // Enemy is in front of player
        {
            double sprite_x = (WINDOW_WIDTH / 2) * (1 + transformed_x / transformed_y);
            int sprite_height = (int)(WINDOW_HEIGHT / current->enemy.dist_to_player);
            
            if (sprite_height > 0 && sprite_x > 0 && sprite_x < WINDOW_WIDTH)
            {
                draw_enemy_sprite(game, &current->enemy, sprite_x, sprite_height);
            }
        }
        
        current = current->next;
    }
}

static bool is_enemy_spawn(char c)
{
    return (c == 'E');
}

static void spawn_enemies_from_map(t_game *game)
{
    int i;
    int j;
    
    i = -1;
    while (++i < game->map.height)
    {
        j = -1;
        while (++j < game->map.width)
        {
            if (j >= (int)ft_strlen(game->map.grid[i]))
                continue;
            if (is_enemy_spawn(game->map.grid[i][j]))
            {
                add_enemy(game, vector_create(i, j));
                game->map.grid[i][j] = '0';
            }
        }
    }
}

void init_enemies(t_game *game)
{
    game->enemies = NULL;
    spawn_enemies_from_map(game);
}
