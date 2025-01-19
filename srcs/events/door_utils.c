/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 20:13:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	is_door(char tile)
{
	return (tile == 'D');
}

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
    //printf("No door found at x:%d y:%d\n", x, y);
    return NULL;
}

bool	is_door_solid(t_game *game, int x, int y)
{
	t_door *door = find_door(game, y, x);  // Inverte x e y
	if (!door)
	{
		return (true);
	}
	if (door->state == DOOR_OPEN && door->animation >= 0.99)
		return (false);
		
	return (true);
}


double	get_player_door_dist(t_door *door, t_vector player_pos)
{
	t_vector	door_pos;

	door_pos = vector_create(door->position.x + 0.5, door->position.y + 0.5);
	return (vector_dist(door_pos, player_pos));
}

void	interact_with_door(t_game *game)
{
	t_door_system	*ds;
	int				i;
	double			distance;

	ds = game->door_system;

	i = -1;
	while (++i < ds->door_count)
	{
		distance = get_player_door_dist(&ds->doors[i], game->p1.pos);
		if (distance <= DOOR_INTERACTION_DISTANCE)
		{
			if (ds->doors[i].state == DOOR_CLOSED && !ds->doors[i].locked)
				ds->doors[i].state = DOOR_OPENING;
			else if (ds->doors[i].state == DOOR_OPEN)
				ds->doors[i].state = DOOR_CLOSING;
			break ;
		}
	}
}
/*
void    interact_with_door(t_game *game)
{
    t_door_system   *ds;
    int             i;
    double          distance;

    ds = game->door_system;
    printf("Tentando interagir com porta. Total de portas: %d\n", ds->door_count);
    
    i = 0;
    while (i < ds->door_count)
    {
        distance = get_player_door_dist(&ds->doors[i], game->p1.pos);
        printf("Porta %d: pos(%d,%d), distancia: %f, estado: %d\n", 
               i, 
               ds->doors[i].position.x, 
               ds->doors[i].position.y,
               distance,
               ds->doors[i].state);
        
        if (distance <= DOOR_INTERACTION_DISTANCE)
        {
            printf("Porta %d está ao alcance!\n", i);
            if (ds->doors[i].state == DOOR_CLOSED && !ds->doors[i].locked)
            {
                printf("Abrindo porta %d\n", i);
                ds->doors[i].state = DOOR_OPENING;
                break;
            }
            else if (ds->doors[i].state == DOOR_OPEN)
            {
                printf("Fechando porta %d\n", i);
                ds->doors[i].state = DOOR_CLOSING;
                break;
            }
        }
        i++;
    }
}*///
/*
void    update_doors(t_game *game)
{
    t_door_system   *ds;
    int             i;
    double          delta_time;

    ds = game->door_system;
    delta_time = 1.0 / 60.0;

    i = -1;
    while (++i < ds->door_count)
    {
        if (ds->doors[i].state == DOOR_OPENING)
        {
            printf("Atualizando porta %d: Abrindo (anim: %f)\n", 
                   i, ds->doors[i].animation);
            ds->doors[i].animation += DOOR_SPEED * delta_time;
            if (ds->doors[i].animation >= 1.0)
            {
                ds->doors[i].animation = 1.0;
                ds->doors[i].state = DOOR_OPEN;
                printf("Porta %d totalmente aberta\n", i);
            }
        }
        else if (ds->doors[i].state == DOOR_CLOSING)
        {
            printf("Atualizando porta %d: Fechando (anim: %f)\n", 
                   i, ds->doors[i].animation);
            ds->doors[i].animation -= DOOR_SPEED * delta_time;
            if (ds->doors[i].animation <= 0.0)
            {
                ds->doors[i].animation = 0.0;
                ds->doors[i].state = DOOR_CLOSED;
                printf("Porta %d totalmente fechada\n", i);
            }
        }
    }
}*/

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
