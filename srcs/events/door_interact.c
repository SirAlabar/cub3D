/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_interact.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/09 11:15:48 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static void	update_door_state(t_door *door)
{
	if (door->locked)
		return ;
	if (door->state == DOOR_CLOSED)
		door->state = DOOR_OPENING;
	else if (door->state == DOOR_OPEN || door->state == DOOR_OPENING
		|| (door->state == DOOR_CLOSING && door->animation > 0.2))
		door->state = DOOR_CLOSING;
}

static t_vector	get_door_position(t_door *door)
{
	return (vector_create(door->position.x + 0.5, door->position.y + 0.5));
}

static int	check_door_distance(t_game *game, t_door *door, double *min_dist)
{
	t_vector	door_pos;
	t_vector	to_door;
	double		dist;
	double		dot;

	door_pos = get_door_position(door);
	to_door = vector_sub(door_pos, game->p1.pos);
	dist = vector_mag(to_door);
	if (dist <= DOOR_INTERACTION_DISTANCE && dist < *min_dist)
	{
		dot = vector_dot(vector_normalize(game->p1.dir),
				vector_normalize(to_door));
		if (dot > 0.9)
		{
			*min_dist = dist;
			return (1);
		}
	}
	return (0);
}

static t_door	*find_closest_door(t_game *game)
{
	int		i;
	t_door	*closest;
	double	min_dist;

	i = 0;
	closest = NULL;
	min_dist = DOOR_INTERACTION_DISTANCE + 1;
	while (i < game->door_system->door_count)
	{
		if (game->door_system->doors[i].active)
			if (check_door_distance(game, &game->door_system->doors[i],
					&min_dist))
				closest = &game->door_system->doors[i];
		i++;
	}
	return (closest);
}

void	interact_with_door(t_game *game)
{
	t_door	*closest_door;

	closest_door = find_closest_door(game);
	if (closest_door)
	{
		update_door_state(closest_door);
		if (game->sounds && game->sounds->is_initialized)
			play_sound(game->sounds->door);
	}
}
