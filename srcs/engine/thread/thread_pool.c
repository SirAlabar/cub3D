/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/28 16:00:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Worker thread function - repeatedly takes tasks from queue and executes them
 */
void *worker_thread(void *arg)
{
    t_thread_pool *pool;
    t_task *task;
    
    pool = (t_thread_pool *)arg;
    
    while (1)
    {
        pthread_mutex_lock(&pool->queue_mutex);
        while (!pool->task_queue && !pool->shutdown)
            pthread_cond_wait(&pool->queue_not_empty, &pool->queue_mutex);
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }
        task = get_task(pool);
        pool->active_threads++;
        pthread_mutex_unlock(&pool->queue_mutex);
        process_task(pool, task);
    }
    return (NULL);
}


/*
 * Initialize thread pool synchronization primitives
 * Returns true if successful, false otherwise
 */
static bool init_pool_sync(t_thread_pool *pool)
{
	if (pthread_mutex_init(&pool->queue_mutex, NULL) != 0)
		return (false);
	if (pthread_cond_init(&pool->queue_not_empty, NULL) != 0)
	{
		pthread_mutex_destroy(&pool->queue_mutex);
		return (false);
	}
	if (pthread_cond_init(&pool->queue_not_full, NULL) != 0)
	{
		pthread_cond_destroy(&pool->queue_not_empty);
		pthread_mutex_destroy(&pool->queue_mutex);
		return (false);
	}
	return (true);
}

/*
 * Create and launch worker threads for the pool
 * Returns true if successful, false otherwise
 */
static bool create_worker_threads(t_thread_pool *pool)
{
	int	i;

	i = 0;
	while (i < pool->thread_count)
	{
		if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0)
		{
			pool->shutdown = true;
			pthread_cond_broadcast(&pool->queue_not_empty);
			return (false);
		}
		i++;
	}
	return (true);
}

/*
 * Creates a new thread pool with the specified number of threads
 */
t_thread_pool *thread_pool_create(int thread_count)
{
	t_thread_pool *pool;

	if (thread_count <= 0)
		thread_count = THREAD_COUNT;
	pool = (t_thread_pool *)calloc(1, sizeof(t_thread_pool));
	if (!pool)
		return (NULL);
	if (!init_pool_sync(pool))
	{
		free(pool);
		return (NULL);
	}
	pool->threads = (pthread_t *)calloc(thread_count, sizeof(pthread_t));
	if (!pool->threads)
	{
		pthread_cond_destroy(&pool->queue_not_full);
		pthread_cond_destroy(&pool->queue_not_empty);
		pthread_mutex_destroy(&pool->queue_mutex);
		free(pool);
		return (NULL);
	}
	pool->thread_count = thread_count;
	pool->max_queue_size = MAX_QUEUE_SIZE;
	if (!create_worker_threads(pool))
	{
		thread_pool_destroy(pool);
		return (NULL);
	}
	return (pool);
}


/*
 * Adds a task to the thread pool's queue
 */
bool thread_pool_add_task(t_thread_pool *pool, t_task_func function, 
    void *argument, t_complete_func on_complete, 
    void *context)
{
    t_task *task;

    if (!pool || !function || pool->shutdown)
        return (false);
    task = task_create(function, argument, on_complete, context);
    if (!task)
        return (false);
    pthread_mutex_lock(&pool->queue_mutex);
    while (pool->queue_size >= pool->max_queue_size && !pool->shutdown)
        pthread_cond_wait(&pool->queue_not_full, &pool->queue_mutex);
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->queue_mutex);
        task_destroy(task);
        return (false);
    }
    if (!pool->task_queue)
    {
        pool->task_queue = task;
        pool->queue_tail = task;
    }
    else
    {
        pool->queue_tail->next = task;
        pool->queue_tail = task;
    }
    pool->queue_size++;
    pthread_cond_signal(&pool->queue_not_empty);
    pthread_mutex_unlock(&pool->queue_mutex);
    return (true);
}
