/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_dda.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/22 21:44:48 by hluiz-ma         ###   ########.fr       */
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

// void	perform_dda(t_ray *ray, t_game *game)
// {
// 	ray->hit = false;
// 	ray->is_door = false;
// 	while (ray->hit == false)
// 	{
// 		if (ray->side_dist.x < ray->side_dist.y)
// 		{
// 			ray->side_dist.x += ray->delta_dist.x;
// 			ray->map_x += ray->step_x;
// 			ray->side = 0;
// 		}
// 		else
// 		{
// 			ray->side_dist.y += ray->delta_dist.y;
// 			ray->map_y += ray->step_y;
// 			ray->side = 1;
// 		}
// 		if (!is_within_map_bounds(game, ray->map_x, ray->map_y))
// 		{
// 			ray->hit = true;
// 			break;
// 		}
		
// 		// Verificar se estamos em um portal
// 		if (game->portal_system && game->portal_system->portal_active)
// 		{
// 			// Nova função para verificar e processar portais
// 			if (process_portal_hit(ray, game))
// 			{
// 				break;  // Sair do loop interno para recomeçar com o raio teleportado
// 			}
// 		}
// 		check_collisions(ray, game);
// 		if (!ray->hit && ray->is_door)
// 			continue ;
// 	}
// 	if (ray->side == 0)
// 		ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
// 	else
// 		ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
// }

void perform_dda(t_ray *ray, t_game *game)
{
    ray->hit = false;
    ray->is_door = false;
    
    // Limitar o número máximo de passagens por portais para evitar loops
    int portal_pass_count = 0;
    const int max_portal_passes = 3;
    
    // Verificar limites iniciais
    if (!is_within_map_bounds(game, ray->map_x, ray->map_y))
    {
        ray->hit = true;
        return;
    }
    
    while (ray->hit == false && portal_pass_count < max_portal_passes)
    {
        // Algoritmo DDA clássico
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
            
            // Verificar limites do mapa
            if (!is_within_map_bounds(game, ray->map_x, ray->map_y))
            {
                ray->hit = true;
                // Definir uma distância segura para evitar artefatos visuais
                if (ray->side == 0)
                    ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
                else
                    ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
                break;
            }
            
            // Verificar se estamos em um portal
            if (game->portal_system && game->portal_system->portal_active)
            {
                // Nova função para verificar e processar portais
                if (process_portal_hit(ray, game))
                {
                    // Verificar novamente os limites após teleportar
                    if (!is_within_map_bounds(game, ray->map_x, ray->map_y))
                    {
                        ray->hit = true;
                        break;
                    }
                    
                    portal_pass_count++;
                    break;  // Sair do loop interno para recomeçar com o raio teleportado
                }
            }
            
            // Verificar colisão comum (parede, porta)
            check_collisions(ray, game);
            
            if (!ray->hit && ray->is_door)
                continue;
        }
        
        // Se não teleportamos, saímos do loop principal
        if (portal_pass_count == 0 || ray->hit)
            break;
    }
    
    // Calcular distância perpendicular à parede
    if (ray->side == 0)
        ray->perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
    else
        ray->perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
        
    // Garantir uma distância mínima para evitar divisão por zero
    if (ray->perp_wall_dist <= 0.001)
        ray->perp_wall_dist = 0.001;
}

void	cast_rays(t_game *game, t_ray *rays)
{
	int	i;

	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		init_ray(&rays[i], game, i);
		step_side_dist(&rays[i], game);
		perform_dda(&rays[i], game);
		wall_height(&rays[i]);
	}
}
