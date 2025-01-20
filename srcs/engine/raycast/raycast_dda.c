/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_dda.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/20 21:34:51 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_ray(t_ray *ray, t_game *game, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->dir.x = game->p1.dir.x + game->p1.plane.x * camera_x;
	ray->dir.y = game->p1.dir.y + game->p1.plane.y * camera_x;
	ray->map_x = (int)game->p1.pos.x;
	ray->map_y = (int)game->p1.pos.y;
	ray->delta_dist.x = fabs(1 / ray->dir.x);
	ray->delta_dist.y = fabs(1 / ray->dir.y);
	ray->hit = false;
	ray->is_door = false;
}

void	step_side_dist(t_ray *ray, t_game *g)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (g->p1.pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - g->p1.pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (g->p1.pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - g->p1.pos.y) * ray->delta_dist.y;
	}
}

void perform_dda(t_ray *ray, t_game *game)
{
    ray->hit = false;
    ray->is_door = false;
    char tile;
   
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
        
        tile = game->map.grid[ray->map_x][ray->map_y];
        
    if (tile == 'D')
    {
        t_door *door = find_door(game, ray->map_x, ray->map_y);
        if (door && door->state != DOOR_OPEN)
        {
            // Ignora a porta se estiver quase totalmente aberta
            if (door->state == DOOR_OPENING && door->animation > 0.95)
                continue;
                
            ray->hit = true;
            ray->is_door = true;
        }
    }
        else if (tile == '1')
        {
            ray->hit = true;
            ray->is_door = false;
        }
    }

    if (ray->side == 0)
        ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
    else
        ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
}
/*
// Adicione este debug no início da sua função perform_dda
void perform_dda(t_ray *ray, t_game *game)
{
    static int debug_count = 0;
    bool should_debug = (++debug_count % 1000 == 0);  // Debug a cada 1000 chamadas

    ray->hit = false;
    ray->is_door = false;
    char tile;

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

        // Verificação de limites
        if (ray->map_x < 0 || ray->map_y < 0 || 
            ray->map_x >= game->map.width || ray->map_y >= game->map.height)
        {
            if (should_debug)
                printf("Error: Ray out of bounds at [%d,%d]\n", ray->map_x, ray->map_y);
            return;
        }

        tile = game->map.grid[ray->map_x][ray->map_y];
        
        // Debug apenas quando encontrar uma porta ou quando should_debug for true
        if ((tile == 'D' || should_debug) && debug_count > 0)
        {
            printf("\n=== Ray Debug (#%d) ===\n", debug_count);
            printf("Position: [%d,%d]\n", ray->map_x, ray->map_y);
            printf("Tile found: %c\n", tile);
            if (tile == 'D')
            {
                printf("Door solid? %s\n", is_door_solid(game, ray->map_x, ray->map_y) ? "yes" : "no");
            }
        }

        if (tile == '1' || (tile == 'D' && is_door_solid(game, ray->map_x, ray->map_y)))
        {
            ray->hit = true;
            ray->is_door = (tile == 'D');
            
            if (ray->is_door && should_debug)
            {
                printf("Door hit confirmed! Position: [%d,%d]\n", ray->map_x, ray->map_y);
            }
        }
    }

    if (ray->side == 0)
        ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
    else
        ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
}*/

void	wall_height(t_ray *ray)
{
	ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_end >= WINDOW_HEIGHT)
		ray->draw_end = WINDOW_HEIGHT - 1;
}

void	cast_rays(t_game *game, t_ray *rays)
{
	int		i;

	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		init_ray(&rays[i], game, i);
		step_side_dist(&rays[i], game);
		perform_dda(&rays[i], game);
		wall_height(&rays[i]);
	}
}
