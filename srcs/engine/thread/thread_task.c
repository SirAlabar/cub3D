
#include <cub3d.h>

/*
 * Creates a new task with the given parameters
 */
t_task *task_create(t_task_func function, void *argument, 
    t_complete_func on_complete, void *context)
{
t_task *task;

if (!function)
return (NULL);
task = (t_task *)malloc(sizeof(t_task));
if (!task)
return (NULL);
task->function = function;
task->argument = argument;
task->on_complete = on_complete;
task->context = context;
task->next = NULL;
return (task);
}


/*
 * Frees memory associated with a task
 */
void task_destroy(t_task *task)
{
    if (task)
        free(task);
}

/*
 * Gets the next task from the queue
 * Must be called with queue_mutex locked
 */
t_task *get_task(t_thread_pool *pool)
{
    t_task *task;
    
    if (!pool || !pool->task_queue)
        return (NULL);
    task = pool->task_queue;
    pool->task_queue = task->next;
    if (!pool->task_queue)
        pool->queue_tail = NULL;
    pool->queue_size--;
    pthread_cond_signal(&pool->queue_not_full);
    return (task);
}

/*
 * Process a task after retrieving it from the queue
 */
void process_task(t_thread_pool *pool, t_task *task)
{
    void *result;
    
    if (!task)
        return;
    result = task->function(task->argument);
    if (task->on_complete)
        task->on_complete(result, task->context);
    task_destroy(task);
    pthread_mutex_lock(&pool->queue_mutex);
    pool->active_threads--;
    pthread_mutex_unlock(&pool->queue_mutex);
}
