/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_dda.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 16:11:46 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/06 18:17:25 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void init_ray(t_ray *ray, t_game *game, int x)
{
    double camera_x;

    camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    ray->dir.x = game->p1.dir.x + game->p1.plane.x * camera_x;
    ray->dir.y = game->p1.dir.y + game->p1.plane.y * camera_x;
    ray->map_x = (int)game->p1.pos.x;
    ray->map_y = (int)game->p1.pos.y;
    ray->delta_dist.x = fabs(1 / ray->dir.x);
    ray->delta_dist.y = fabs(1 / ray->dir.y);
    ray->hit = false;    
}

void step_side_dist(t_ray *ray, t_game *game)
{
    if (ray->dir.x < 0)
    {
        ray->step_x = -1;
        ray->side_dist.x = (game->p1.pos.x - ray->map_x) * ray->delta_dist.x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist.x = (ray->map_x + 1.0 - game->p1.pos.x) * ray->delta_dist.x;
    }
    if (ray->dir.y < 0)
    {
        ray->step_y = -1;
        ray->side_dist.y = (game->p1.pos.y - ray->map_y) * ray->delta_dist.y; 
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist.y = (ray->map_y + 1.0 - game->p1.pos.y) * ray->delta_dist.y;
    }
}

void perform_dda(t_ray *ray, t_game *game)
{
    while (ray->hit == false)
    {
        if (ray->side_dist.x < ray->side_dist.y)
        {
            ray->side_dist.x += ray->delta_dist.x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else
        {
            ray->side_dist.y += ray->delta_dist.y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
        if (game->map.grid[ray->map_x][ray->map_y] == '1')
            ray->hit = true;
    }
    if (ray->side == 0)
        ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
    else
        ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
}

void wall_height(t_ray *ray)
{
    ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
    ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
    if (ray->draw_start < 0)
        ray->draw_start = 0;
    ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
    if (ray->draw_end >= WINDOW_HEIGHT)
        ray->draw_end = WINDOW_HEIGHT - 1;
}

void cast_rays(t_game *game)
{
    int i;
    t_ray ray;

    i = -1;
    while(++i < WINDOW_WIDTH)
    {
        init_ray(&ray, game, i);
        step_side_dist(&ray, game);
        perform_dda(&ray, game);
        wall_height(&ray);
        draw_wall(game, &ray, i);
    }
}
