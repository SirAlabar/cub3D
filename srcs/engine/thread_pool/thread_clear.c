
#include <cub3d.h>


/*
 * Destroys a thread pool and frees all associated resources
 */
void thread_pool_destroy(t_thread_pool *pool)
{
    t_task *task;
    t_task *next_task;
    int i;
    
    if (!pool)
        return;
    pthread_mutex_lock(&pool->queue_mutex);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->queue_not_empty);
    pthread_mutex_unlock(&pool->queue_mutex);
    for (i = 0; i < pool->thread_count; i++)
        pthread_join(pool->threads[i], NULL);
    free(pool->threads);
    task = pool->task_queue;
    while (task)
    {
        next_task = task->next;
        task_destroy(task);
        task = next_task;
    }
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->queue_not_empty);
    pthread_cond_destroy(&pool->queue_not_full);
    free(pool);
}
