/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_raycast.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:10:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/07 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Verifica se o raio atingiu um portal e trata a teleportação
** Retorna true se o raio foi teleportado
*/
bool process_portal_hit(t_ray *ray, t_game *game)
{
    t_portal_wall *entry;
    t_portal_wall *exit;
    
    // Verificar colisão com portal azul
    if (check_portal_hit(ray, game, &game->portal_system->blue_portal))
    {
        entry = &game->portal_system->blue_portal;
        if (entry->linked_portal && entry->linked_portal->active)
        {
            exit = entry->linked_portal;
            teleport_ray(ray, game, entry, exit);
            return (true);
        }
    }
    // Verificar colisão com portal laranja
    else if (check_portal_hit(ray, game, &game->portal_system->orange_portal))
    {
        entry = &game->portal_system->orange_portal;
        if (entry->linked_portal && entry->linked_portal->active)
        {
            exit = entry->linked_portal;
            teleport_ray(ray, game, entry, exit);
            return (true);
        }
    }
    
    return (false);
}

/*
** Teleporta um raio de um portal para outro
*/
void teleport_ray(t_ray *ray, t_game *game, t_portal_wall *entry, t_portal_wall *exit)
{
    t_vector adj_pos;
    t_vector adj_dir;
    double angle;
    
    // Calcular posição ajustada após o portal
    adj_pos = vector_create(
        exit->position.x + exit->normal.x * 0.5 + 0.5,
        exit->position.y + exit->normal.y * 0.5 + 0.5
    );
    
    // Verificar se a posição está dentro dos limites do mapa
    if (!is_within_map_bounds(game, (int)adj_pos.x, (int)adj_pos.y))
    {
        // Se estiver fora dos limites, marcar como hit e retornar
        ray->hit = true;
        return;
    }
    
    // Calcular ângulo entre portais
    angle = get_angle_between_normals(entry->normal, exit->normal);
    
    // Rotacionar direção do raio
    adj_dir = vector_rotate(ray->dir, angle);
    
    // Atualizar propriedades do raio
    ray->dir = adj_dir;
    ray->map_x = (int)adj_pos.x;
    ray->map_y = (int)adj_pos.y;
    ray->delta_dist.x = fabs(1 / ray->dir.x);
    ray->delta_dist.y = fabs(1 / ray->dir.y);
    
    // Reinicializar variáveis para DDA
    step_side_dist(ray, game);
    ray->hit = false;
    ray->is_door = false;
    if (ray->side == 0)
        ray->side_dist.x += 0.5 * ray->delta_dist.x;
    else
        ray->side_dist.y += 0.5 * ray->delta_dist.y;
}