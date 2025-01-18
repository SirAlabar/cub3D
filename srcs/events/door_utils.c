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

t_door	*find_door(t_game *game, int x, int y)
{
	t_door_system	*ds;
	int				i;

	ds = game->door_system;
	i = 0;
	while (i < ds->door_count)
	{
		if (ds->doors[i].position.x == x && ds->doors[i].position.y == y)
			return (&ds->doors[i]);
		i++;
	}
	return (NULL);
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
	i = 0;
	while (i < ds->door_count)
	{
		distance = get_player_door_dist(&ds->doors[i], game->p1.pos);
		if (distance <= DOOR_INTERACTION_DISTANCE && 
			ds->doors[i].state == DOOR_CLOSED)
		{
			if (!ds->doors[i].locked)
				ds->doors[i].state = DOOR_OPENING;
			break ;
		}
		i++;
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
