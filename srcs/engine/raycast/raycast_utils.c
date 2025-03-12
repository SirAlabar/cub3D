/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 20:52:40 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:24:17 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void check_collisions(t_ray *ray, t_game *game)
{
    char tile;

    tile = game->map.grid[ray->map_x][ray->map_y];
    if (tile == '1')
    {
        handle_wall_collision(ray);
        return;
    }
    
    if (tile == 'D')
    {
		
        // Sempre processa o tile da porta, mesmo que esteja aberta
        // Isso permitirá que as molduras sejam renderizadas
        handle_door_collision(ray, game);
        
        // Se o raio não atingiu nada (porta aberta), continuar o raycasting
        if (!ray->hit)
            update_ray_position(ray);
    }
}

void	handle_wall_collision(t_ray *ray)
{
	ray->hit = true;
	ray->is_door = false;
	if (ray->side == 0)
		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
}

void	set_door_hit(t_ray *ray, double orig_dist)
{
	ray->hit = true;
	ray->perp_wall_dist = orig_dist;
}

double	get_wall_x(t_ray *ray, t_game *game, double orig_dist)
{
	double	wallx;

	if (ray->side == 0)
		wallx = game->p1.pos.y + orig_dist * ray->dir.y;
	else
		wallx = game->p1.pos.x + orig_dist * ray->dir.x;
	return (wallx - floor(wallx));
}

void handle_door_side_0(t_ray *ray, double *orig_dist)
{
    ray->side_dist.x -= ray->delta_dist.x / 2;
    if (ray->side_dist.x > ray->side_dist.y)
    {
        ray->side_dist.y += ray->delta_dist.y;
        ray->map_y += ray->step_y;
        ray->side = 1;
        *orig_dist = ray->side_dist.y - ray->delta_dist.y;
        ray->is_door = 0;
        ray->is_doorwall = 1;
    }
    else
    {
        *orig_dist = ray->side_dist.x - ray->delta_dist.x;
        ray->is_door = 1;
        ray->is_doorwall = 0;
    }
    ray->side_dist.x += ray->delta_dist.x;
}

void handle_door_side_1(t_ray *ray, double *orig_dist)
{
    ray->side_dist.y -= ray->delta_dist.y / 2;
    if (ray->side_dist.y > ray->side_dist.x)
    {
        ray->side_dist.x += ray->delta_dist.x;
        ray->map_x += ray->step_x;
        ray->side = 0;
        *orig_dist = ray->side_dist.x - ray->delta_dist.x;
        ray->is_door = 0;
        ray->is_doorwall = 1;
    }
    else
    {
        *orig_dist = ray->side_dist.y - ray->delta_dist.y;
        ray->is_door = 1;
        ray->is_doorwall = 0;
    }
    ray->side_dist.y += ray->delta_dist.y;
}

// void process_door_animation(t_ray *ray, t_game *game, t_door *door, double orig_dist)
// {
//     double wallx;
//     double door_hit_pos;
//     if (ray->is_door)
//     {
//         wallx = get_wall_x(ray, game, orig_dist);
//         door_hit_pos = wallx;
//         if (door_hit_pos > door->animation)
//             set_door_hit(ray, orig_dist);
//     }
//     else if (ray->is_doorwall)
//     {
//         set_door_hit(ray, orig_dist);
//     }
// }
// if (door->animation > 0.9 && door_hit_pos > door->animation)
// {
// 	update_ray_position(ray);
// 	return ;
// }
// if (door_hit_pos <= door->animation)
// {
// 	update_ray_position(ray);
// 	return ;
// }

void process_door_animation(t_ray *ray, t_game *game, t_door *door, double orig_dist)
{
    double wallx;
    
    if (!ray->is_door)
    {
        // Se for a moldura da porta, sempre define hit
        set_door_hit(ray, orig_dist);
        return;
    }
	if (door->state == DOOR_CLOSED)
	{
		set_door_hit(ray, orig_dist);
		return ;
	}
    // Obter a posição exata de hit na porta (valor entre 0 e 1)
    wallx = get_wall_x(ray, game, orig_dist);
	if (wallx > door->animation)
		set_door_hit(ray, orig_dist);
	else
		update_ray_position(ray);
}

void handle_door_collision(t_ray *ray, t_game *game)
{
    t_door *door;
    double orig_dist;

    door = find_door(game, ray->map_x, ray->map_y);
    if (!door)
        return;
    if (ray->side == 0)
        handle_door_side_0(ray, &orig_dist);
    else
        handle_door_side_1(ray, &orig_dist);
    
    process_door_animation(ray, game, door, orig_dist);
}