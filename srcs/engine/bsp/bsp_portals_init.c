/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_portal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/01 16:00:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
** Identify if a BSP line is a portal to another sector
** Returns true if line connects to a different sector
*/
bool	is_portal(t_bsp_line *line)
{
	if (!line)
		return (false);
	if (line->neighbor_sector_id >= 0 && line->neighbor_sector_id != line->sector_id)
		return (true);
	return (false);
}

/*
** Get sector of a BSP line
** Returns the sector ID for the line
*/
int	get_line_sector(t_bsp_line *line)
{
	if (!line)
		return (-1);
	return (line->sector_id);
}

/*
** Get neighbor sector of a BSP line
** Returns the neighbor sector ID for the line
*/
int	get_line_neighbor(t_bsp_line *line)
{
	if (!line)
		return (-1);
	return (line->neighbor_sector_id);
}

/*
** Create a new portal queue
** Allocates and initializes a new queue structure
*/
t_portal_queue	*create_portal_queue(void)
{
	t_portal_queue	*queue;
	
	queue = (t_portal_queue *)malloc(sizeof(t_portal_queue));
	if (!queue)
		return (NULL);
	
	init_portal_queue(queue);
	return (queue);
}

/*
** Free portal queue resources
*/
void	free_portal_queue(t_portal_queue *queue)
{
	if (queue)
		free(queue);
}

