
#include <cub3d.h>

/*
 * Returns the number of pending tasks in the queue
 */
int thread_pool_pending_tasks(t_thread_pool *pool)
{
    int size;
    
    if (!pool)
        return (0);
    pthread_mutex_lock(&pool->queue_mutex);
    size = pool->queue_size;
    pthread_mutex_unlock(&pool->queue_mutex);
    return (size);
}

/*
 * Returns the number of worker threads in the pool
 */
int thread_pool_thread_count(t_thread_pool *pool)
{
    if (!pool)
        return (0);
    return (pool->thread_count);
}

/*
 * Waits until all currently queued tasks are completed
 * Uses timeout to prevent indefinite waiting
 */
void thread_pool_wait(t_thread_pool *pool)
{
    struct timespec timeout;
    
    if (!pool)
        return;
    
    pthread_mutex_lock(&pool->queue_mutex);
    while (pool->queue_size > 0 || pool->active_threads > 0)
    {
        set_timeout(&timeout, WAIT_TIMEOUT_MS);
        pthread_cond_timedwait(
            &pool->queue_not_full, 
            &pool->queue_mutex, 
            &timeout
        );
    }
    pthread_mutex_unlock(&pool->queue_mutex);
}

/*
 * Waits until all currently queued tasks are completed with a maximum timeout
 * Returns true if all tasks completed, false if timeout occurred
 */
bool thread_pool_wait_timeout(t_thread_pool *pool, t_fixed64 timeout_ms)
{
    struct timespec timeout;
    t_fixed64 start_time;
    t_fixed64 current_time;
    t_fixed64 elapsed;
    t_fixed64 next_wait;
    
    if (!pool)
        return (false);
    start_time = ((t_fixed64)get_time_ms() << FIXED_POINT_BITS) / 1000;
    pthread_mutex_lock(&pool->queue_mutex);
    while (pool->queue_size > 0 || pool->active_threads > 0)
    {
        current_time = ((t_fixed64)get_time_ms() << FIXED_POINT_BITS) / 1000;
        elapsed = current_time - start_time;
        if (elapsed >= timeout_ms)
        {
            pthread_mutex_unlock(&pool->queue_mutex);
            return (false);
        }
        next_wait = timeout_ms - elapsed;
        if (next_wait > WAIT_TIMEOUT_MS_FIXED)
            next_wait = WAIT_TIMEOUT_MS_FIXED;
            
        set_timeout(&timeout, next_wait);
        pthread_cond_timedwait(
            &pool->queue_not_full, 
            &pool->queue_mutex, 
            &timeout
        );
    }
    pthread_mutex_unlock(&pool->queue_mutex);
    return (true);
}
