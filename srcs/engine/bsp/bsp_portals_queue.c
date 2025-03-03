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
** Initialize empty portal queue
** Sets up head and tail pointers for empty queue
*/
void init_portal_queue(t_portal_queue *queue)
{
    if (!queue)
        return;
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
}

/*
** Check if portal queue is empty
** Returns true if no items in queue
*/
bool	is_queue_empty(t_portal_queue *queue)
{
	if (!queue)
		return (true);
	if (queue->count == 0)
		return (true);
	return (false);
}

/*
** Check if portal queue is full
** Returns true if queue has reached maximum capacity
*/
bool	is_queue_full(t_portal_queue *queue)
{
	if (!queue)
		return (true);
	if (queue->count >= MAX_PORTAL_QUEUE)
		return (true);
	return (false);
}

/*
** Add new portal to queue
** Returns false if queue is full or parameters invalid
*/
bool	enqueue_portal(t_portal_queue *queue, int sector_id, int x_min, 
    int x_max, int depth)
{
    if (!queue || is_queue_full(queue) || depth >= MAX_PORTAL_DEPTH)
        return (false);
    queue->items[queue->tail].sector_id = sector_id;
    queue->items[queue->tail].x_min = x_min;
    queue->items[queue->tail].x_max = x_max;
    queue->items[queue->tail].depth = depth;
    queue->tail = (queue->tail + 1) % MAX_PORTAL_QUEUE;
    queue->count++;
    return (true);
}

/*
** Remove and return next portal from queue
** Returns false if queue is empty or parameters invalid
*/
bool	dequeue_portal(t_portal_queue *queue, t_portal_item *item)
{
	if (!queue || !item || is_queue_empty(queue))
		return (false);
        *item = queue->items[queue->head];
        queue->head = (queue->head + 1) % MAX_PORTAL_QUEUE;
        queue->count--;
	return (true);
}
