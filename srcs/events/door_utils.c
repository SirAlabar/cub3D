/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/20 21:37:01 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_door(char tile)
{ 
	return (tile == 'D');
}
/*
t_door *find_door(t_game *game, int x, int y)
{
    int i;

    i = 0;
    while (i < game->door_system->door_count)
    {
        if (game->door_system->doors[i].position.x == x &&
            game->door_system->doors[i].position.y == y)
        {
            return &game->door_system->doors[i];
        }
        i++;
    }
    return NULL;
}*/

t_door *find_door(t_game *game, int x, int y)
{
    
    int i = 0;
    while (i < game->door_system->door_count)
    {
        
        if (game->door_system->doors[i].position.x == x &&
            game->door_system->doors[i].position.y == y)
        {
            return &game->door_system->doors[i];
        }
        i++;
    }
    return NULL;
}
/*
bool	is_door_solid(t_game *game, int x, int y)
{
	t_door *door = find_door(game, y, x);
	if (!door)
	{
		return (true);
	}
	if (door->state == DOOR_OPEN && door->animation >= 0.99)
		return (false);
		
	return (true);
}*/

bool is_door_solid(t_game *game, int x, int y)
{
    t_door *door = find_door(game, x, y);
    
    if (!door)
    {
        return true;
    }
    
    // Se a porta estiver aberta ou quase aberta, não é sólida
    if (door->state == DOOR_OPEN && door->animation >= 0.99)
    {
        return false;
    }
    return true;
}

double    get_player_door_dist(t_door *door, t_vector player_pos)
{
    t_vector    door_pos;
    double      dist;

    door_pos = vector_create(door->position.x + 0.5, door->position.y + 0.5);
    double dx = fabs(door_pos.x - player_pos.x);
    double dy = fabs(door_pos.y - player_pos.y);
    dist = fmin(dx, dy);
    return (dist);
}

void    interact_with_door(t_game *game)
{
    t_door_system    *ds;
    int              i;
    double          distance;

    ds = game->door_system;
    i = -1;
    while (++i < ds->door_count)
    {
        distance = get_player_door_dist(&ds->doors[i], game->p1.pos);
        if (distance <= DOOR_INTERACTION_DISTANCE)
        {
            t_door *door = &ds->doors[i];
            if (door->state == DOOR_CLOSED && !door->locked)
            {
                door->state = DOOR_OPENING;
                printf("Opening door %d\n", i);
                return;
            }
            else if (door->state == DOOR_OPEN || 
                    door->state == DOOR_OPENING ||
                    (door->state == DOOR_CLOSING && door->animation > 0.2))
            {
                door->state = DOOR_CLOSING;
                return;
            }
        }
    }
}

void	calculate_door_dimensions(t_ray *ray, int *height, int *start, int *end)
{
	double	perp_wall_dist;

	if (ray->side == 0)
		perp_wall_dist = ray->side_dist.x - ray->delta_dist.x;
	else
		perp_wall_dist = ray->side_dist.y - ray->delta_dist.y;
	*height = (int)(WINDOW_HEIGHT / perp_wall_dist);
	*start = -(*height) / 2 + WINDOW_HEIGHT / 2;
	*end = (*height) / 2 + WINDOW_HEIGHT / 2;
	if (*start < 0)
		*start = 0;
	if (*end >= WINDOW_HEIGHT)
		*end = WINDOW_HEIGHT - 1;
}

double	get_wall_hit_position(t_game *game, t_ray *ray)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	return (wall_x);
}
