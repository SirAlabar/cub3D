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
// bool	process_portal_hit(t_ray *ray, t_game *game)
// {
// 	t_portal_wall	*entry;
// 	t_portal_wall	*exit;

// 	if (check_portal_hit(ray, game, &game->portal_system->blue_portal))
// 	{
// 		entry = &game->portal_system->blue_portal;
// 		if (entry->linked_portal && entry->linked_portal->active)
// 		{
// 			exit = entry->linked_portal;
// 			teleport_ray(ray, game, entry, exit);
// 			return (true);
// 		}
// 	}
// 	else if (check_portal_hit(ray, game, &game->portal_system->orange_portal))
// 	{
// 		entry = &game->portal_system->orange_portal;
// 		if (entry->linked_portal && entry->linked_portal->active)
// 		{
// 			exit = entry->linked_portal;
// 			teleport_ray(ray, game, entry, exit);
// 			return (true);
// 		}
// 	}
// 	return (false);
// }
/*
** Verifica se existe um portal ativo na posição e direção especificadas
*/
t_portal_wall	*find_portal_at_position(t_game *game, int x, int y, t_cardinal card)
{
	if (game->portal_system->blue_portal.active && 
		game->portal_system->blue_portal.position.x == x && 
		game->portal_system->blue_portal.position.y == y && 
		game->portal_system->blue_portal.card == card)
		return (&game->portal_system->blue_portal);
	
	if (game->portal_system->orange_portal.active && 
		game->portal_system->orange_portal.position.x == x && 
		game->portal_system->orange_portal.position.y == y && 
		game->portal_system->orange_portal.card == card)
		return (&game->portal_system->orange_portal);
	
	return (NULL);
}

bool process_portal_hit(t_ray *ray, t_game *game)
{
    t_portal_wall *portal;
    t_cardinal card;
    static int teleport_count = 0;
    static int debug_counter = 0;
    
    /* Limitar recursão */
    if (teleport_count > 5)
    {
        if (debug_counter++ % 10000 == 0)
            printf("DEBUG: Maximum portal recursion depth reached\n");
        return (false);
    }
    
    /* Determinar direção cardinal */
    card = ray->side == 0 ? 
        (ray->dir.x > 0 ? WEST : EAST) : 
        (ray->dir.y > 0 ? SOUTH : NORTH);
    
    /* Buscar portal na posição atual */
    portal = find_portal_at_position(game, ray->map_x, ray->map_y, card);
    
    /* Se encontrou portal ativo e vinculado */
    if (portal && portal->linked_portal && portal->linked_portal->active)
    {
        /* Registrar portal no raio para renderização posterior */
        ray->hit_portal = portal;
        
        if (debug_counter++ % 10000 == 0)
            printf("DEBUG: Ray hit portal at (%d,%d), side=%d\n", 
                ray->map_x, ray->map_y, ray->side);
        
        /* Traduzir raio para continuar raycasting */
        teleport_count++;
        translate_portal_ray(ray, game, portal, portal->linked_portal);
        teleport_count--;
        
        return (true);
    }
    
    return (false);
}
///////////////////////////////////////////////////////////////

static void	calculate_teleport_angle(t_portal_wall *entry, t_portal_wall *exit,
	double *angle)
{
	*angle = 0;
	if (entry->card == get_opposite_cardinal(exit->card))
		*angle = M_PI;
	else if ((entry->card == NORTH && exit->card == EAST)
		|| (entry->card == EAST && exit->card == SOUTH)
		|| (entry->card == SOUTH && exit->card == WEST)
		|| (entry->card == WEST && exit->card == NORTH))
		*angle = M_PI / 2;
	else
		*angle = -M_PI / 2;
}

void	calculate_ray_steps(t_ray *ray, t_vector adj_pos)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (ray->map_x - adj_pos.x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - adj_pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (ray->map_y - adj_pos.y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - adj_pos.y) * ray->delta_dist.y;
	}
	
	/* Evitar valores negativos */
	if (ray->side_dist.x < 0)
		ray->side_dist.x = 0;
	if (ray->side_dist.y < 0)
		ray->side_dist.y = 0;
}

void teleport_ray_update_position(t_ray *ray, t_vector adj_pos)
{
    /* Atualizar posição do mapa */
    ray->map_x = (int)adj_pos.x;
    ray->map_y = (int)adj_pos.y;
    
    /* Recalcular delta_dist baseado na nova direção */
    ray->delta_dist.x = fabs(1.0 / ray->dir.x);
    ray->delta_dist.y = fabs(1.0 / ray->dir.y);
    
    /* Evitar divisão por zero */
    if (ray->delta_dist.x < 0.0001)
        ray->delta_dist.x = 0.0001;
    if (ray->delta_dist.y < 0.0001)
        ray->delta_dist.y = 0.0001;
    
    /* Calcular passos e distâncias laterais */
    calculate_ray_steps(ray, adj_pos);
    
    /* Resetar flags */
    ray->hit = false;
    ray->is_door = false;
    
    /* Definir distância perpendicular inicial para evitar colisões imediatas */
    ray->perp_wall_dist = 0.1;
}

void translate_portal_ray(t_ray *ray, t_game *game, t_portal_wall *entry,
	t_portal_wall *exit)
{
	t_vector exit_dir;
	t_vector adj_pos;
	double angle;
	static int debug_counter = 0;
	
	// Obter a direção de saída do portal de destino
	exit_dir = get_direction_vector(exit->card);
	
	// Calcular a posição ajustada na frente do portal de saída
	adj_pos.x = exit->position.x + exit_dir.x * 0.2 + 0.5;
	adj_pos.y = exit->position.y + exit_dir.y * 0.2 + 0.5;
	
	// Verificar limites do mapa
	if (!is_within_map_bounds(game, (int)adj_pos.x, (int)adj_pos.y))
	{
		ray->hit = true;
		return;
	}
	
	// Calcular ângulo entre os portais
	calculate_teleport_angle(entry, exit, &angle);
	
	// Rotacionar a direção do raio usando a função de vetor existente
	ray->dir = vector_normalize(vector_rotate(ray->dir, angle));
	
	// Atualizar a posição do raio
	ray->map_x = (int)adj_pos.x;
	ray->map_y = (int)adj_pos.y;
	
	// Recalcular delta_dist após a rotação
	ray->delta_dist.x = fabs(1.0 / ray->dir.x);
	ray->delta_dist.y = fabs(1.0 / ray->dir.y);
	
	// Proteger contra divisão por zero
	if (ray->delta_dist.x < 0.0001)
		ray->delta_dist.x = 0.0001;
	if (ray->delta_dist.y < 0.0001)
		ray->delta_dist.y = 0.0001;
	
	// Calcular passos e distâncias laterais para o novo ponto
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (ray->map_x - adj_pos.x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - adj_pos.x) * ray->delta_dist.x;
	}
	
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (ray->map_y - adj_pos.y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - adj_pos.y) * ray->delta_dist.y;
	}
	
	// Evitar valores negativos
	if (ray->side_dist.x < 0)
		ray->side_dist.x = 0;
	if (ray->side_dist.y < 0)
		ray->side_dist.y = 0;
	
	// Resetar flags para continuar o DDA
	ray->hit = false;
	ray->is_door = false;
	
	if (debug_counter++ % 10000 == 0)
	{
		printf("DEBUG: Translated ray to (%d,%d), dir=(%.2f,%.2f)\n", 
			ray->map_x, ray->map_y, ray->dir.x, ray->dir.y);
		printf("DEBUG: Angle=%.2f degrees\n", angle * 180.0 / M_PI);
	}
}