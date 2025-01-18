/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:55:29 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/11 13:55:31 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void set_impact_position(t_game *game)
{
    t_ray *ray;

    ray = &game->rays[WINDOW_WIDTH / 2];
    if (ray->side == 0)
    {
        game->shot.impact.pos.x = ray->map_x;
        if (ray->step_x < 0)
            game->shot.impact.pos.x += 1.0;
        game->shot.impact.pos.y = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
    }
    else
    {
        game->shot.impact.pos.y = ray->map_y;
        if (ray->step_y < 0)
            game->shot.impact.pos.y += 1.0;
        game->shot.impact.pos.x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
    }
}

void debug_ray(t_ray *ray, t_game *game)
{
    printf("Ray Debug:\n");
    printf("Map Position: (%d, %d)\n", ray->map_x, ray->map_y);
    printf("Step: (%d, %d)\n", ray->step_x, ray->step_y);
    printf("Direction: (%f, %f)\n", ray->dir.x, ray->dir.y);
    printf("Perpendicular Distance: %f\n", ray->perp_wall_dist);

    if (ray->hit)
    {
        double impact_x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
        double impact_y = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;

        printf("Impact Position: (%f, %f)\n", impact_x, impact_y);
        int screen_x = (int)((WINDOW_WIDTH / 2) * 
            (1 + (impact_x - game->p1.pos.x) / ray->perp_wall_dist));
        int screen_y = (int)((WINDOW_HEIGHT / 2) * 
            (1 + (impact_y - game->p1.pos.y) / ray->perp_wall_dist));

        printf("Screen Position: (%d, %d)\n", screen_x, screen_y);
    }
    else
    {
        printf("No impact detected.\n");
    }
}

void fire_shot(t_game *game)
{
    t_ray ray;

    if (game->shot.active)
        return ;

    init_ray(&ray, game, WINDOW_WIDTH / 2);
    debug_ray(&ray, game);
    step_side_dist(&ray, game);
    perform_dda(&ray, game);
    debug_ray(&ray, game);

    game->shot.active = true;
    game->shot.impact.is_enemy = false;
    game->shot.ray = ray;
    if (ray.hit)
    {
        game->shot.impact.active = true;
        game->shot.impact.time = get_time_ms();
        game->shot.impact.frame = 0;
        if (ray.side == 0)
        {
            game->shot.impact.pos.x = ray.map_x;
            if (ray.step_x < 0)
                game->shot.impact.pos.x += 1.0;
            game->shot.impact.pos.y = game->p1.pos.y + ray.perp_wall_dist * ray.dir.y;
        }
        else
        {
            game->shot.impact.pos.y = ray.map_y;
            if (ray.step_y < 0)
                game->shot.impact.pos.y += 1.0;
            game->shot.impact.pos.x = game->p1.pos.x + ray.perp_wall_dist * ray.dir.x;
        }
    }
}



void update_shot_system(t_game *game)
{
    if (game->p1.is_firing && !game->shot.active)
        fire_shot(game);
    if (game->shot.impact.active)
        draw_impact(game);
}


void get_impact_bounds(t_game *game, t_vector_i *start, t_vector_i *end,
    int impact_size)
{
    t_vector sprite_pos;
    t_vector transform;
    double inv_det;
    int sprite_screen_x;

    sprite_pos = vector_sub(game->shot.impact.pos, game->p1.pos);
    inv_det = 1.0 / vector_dot(game->p1.plane, vector_create(-game->p1.dir.y,
                game->p1.dir.x));
    transform.x = inv_det * vector_dot(game->p1.dir, vector_create(sprite_pos.y,
                -sprite_pos.x));
    transform.y = inv_det * vector_dot(game->p1.plane,
            vector_create(-sprite_pos.x, -sprite_pos.y));

    sprite_screen_x = (int)(WINDOW_WIDTH / 2 * (1 + transform.x / transform.y));
    start->x = sprite_screen_x - impact_size / 2;
    end->x = sprite_screen_x + impact_size / 2;
    start->y = WINDOW_HEIGHT / 2 - impact_size / 2;
    end->y = WINDOW_HEIGHT / 2 + impact_size / 2;
}
