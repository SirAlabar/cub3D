/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:00:15 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 17:37:42 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/**
 * This function was referenced in enemy_utils2.c but wasn't defined in the shared code.
 * From the usage, it appears to initialize ray data for tracing to an enemy position.
 */
t_ray_data init_ray_data(t_game *game, t_vector enemy_pos)
{
    t_ray_data ray;
    t_vector ray_dir;

    // Calculate direction vector from player to enemy
    ray_dir = vector_normalize(vector_sub(enemy_pos, game->p1.pos));
    
    // Set ray starting position to player position
    ray.pos = game->p1.pos;
    
    // Set initial map position (the grid cell the player is in)
    ray.map_x = (int)ray.pos.x;
    ray.map_y = (int)ray.pos.y;
    
    // Calculate delta distances
    ray.delta_dist.x = fabs(1 / ray_dir.x);
    ray.delta_dist.y = fabs(1 / ray_dir.y);
    
    // Calculate ray steps and initial side distances
    calculate_ray_step(&ray, ray_dir);
    
    return ray;
}

/**
 * This function was referenced in enemy_utils.c but wasn't defined in the shared code.
 * From the name and usage, it appears to draw a vertical stripe of an enemy sprite.
 */
void draw_enemy_stripe(t_game *game, t_enemy *enemy, t_sprite_data *s, int stripe)
{
    t_draw_params p;
    int d;

    p.game = game;
    p.enemy = enemy;
    p.pos.x = stripe;
    
    // Calculate texture X coordinate
    p.tex.x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
            * enemy->texture->width / s->width) / 256;
    
    // Only draw if texture X is valid
    if (p.tex.x >= 0 && p.tex.x < enemy->texture->width)
    {
        // Draw each vertical pixel in the stripe
        p.pos.y = s->start_y;
        while (p.pos.y < s->end_y)
        {
            // Calculate texture Y coordinate
            d = (p.pos.y) * 256 - WINDOW_HEIGHT * 128 + s->height * 128;
            p.tex.y = ((d * enemy->texture->height) / s->height) / 256;
            
            // Only draw if texture Y is valid
            if (p.tex.y >= 0 && p.tex.y < enemy->texture->height)
                draw_stripe_color(&p);
            
            p.pos.y++;
        }
    }
}
void calculate_ray_step(t_ray_data *ray, t_vector ray_dir)
{
    if (ray_dir.x < 0)
    {
        ray->step.x = -1;
        ray->side_dist.x = (ray->pos.x - ray->map_x) * ray->delta_dist.x;
    }
    else
    {
        ray->step.x = 1;
        ray->side_dist.x = (ray->map_x + 1.0 - ray->pos.x) * ray->delta_dist.x;
    }
    
    if (ray_dir.y < 0)
    {
        ray->step.y = -1;
        ray->side_dist.y = (ray->pos.y - ray->map_y) * ray->delta_dist.y;
    }
    else
    {
        ray->step.y = 1;
        ray->side_dist.y = (ray->map_y + 1.0 - ray->pos.y) * ray->delta_dist.y;
    }
}
void draw_stripe_color(t_draw_params *p)
{
    int color;

    color = get_texture_pixel(p->enemy->texture, p->tex.x, p->tex.y);
    
    // Only draw non-transparent pixels (assume color 0x00FFFFFF is transparent)
    if ((color & 0x00FFFFFF) != 0)
    {
        // Apply shading based on distance
        draw_pixel(p->game, p->pos.x, p->pos.y,
            apply_shade(color, 1.0 / (1.0 + p->enemy->dist_to_player * 0.04)));
    }
}
