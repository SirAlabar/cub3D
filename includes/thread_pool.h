/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/28 16:00:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_POOL_H
# define THREAD_POOL_H

# include <types.h>

# define THREAD_COUNT 8
# define MAX_QUEUE_SIZE 256
# define WAIT_TIMEOUT_MS 100
# define WAIT_TIMEOUT_MS_FIXED 6553600

/*
 * Function pointer types for task callbacks
 * task_func: Main function to execute in thread
 * complete_func: Optional callback when task completes
 */
typedef void *(*t_task_func)(void *arg);
typedef void (*t_complete_func)(void *result, void *context);

/*
 * Task structure representing a unit of work
 * to be processed by the thread pool
 */
typedef struct s_task
{
    t_task_func      function;
    void            *argument; 
    t_complete_func  on_complete;  
    void            *context;
    struct s_task   *next;     
} t_task;

/*
 * Thread pool data structure
 * Manages a collection of worker threads and task queue
 */
typedef struct s_thread_pool
{
    pthread_t          *threads;
    int                 thread_count;
    t_task             *task_queue;
    t_task             *queue_tail;
    int                 queue_size; 
    int                 max_queue_size;
    pthread_mutex_t     queue_mutex; 
    pthread_cond_t      queue_not_empty;
    pthread_cond_t      queue_not_full; 
    bool                shutdown;
    int                 active_threads;
} t_thread_pool;

/*
 ** Thread pool functions
 */


t_task *task_create(t_task_func function, void *argument, t_complete_func on_complete, void *context);
void task_destroy(t_task *task);
t_task *get_task(t_thread_pool *pool);
void process_task(t_thread_pool *pool, t_task *task);
void *worker_thread(void *arg);
t_thread_pool *thread_pool_create(int thread_count);
bool thread_pool_add_task(t_thread_pool *pool, t_task_func function, void *argument, t_complete_func on_complete, void *context);
void thread_pool_destroy(t_thread_pool *pool);
int thread_pool_pending_tasks(t_thread_pool *pool);
int thread_pool_thread_count(t_thread_pool *pool);
void thread_pool_wait(t_thread_pool *pool);
bool thread_pool_wait_timeout(t_thread_pool *pool, t_fixed64 timeout_ms);
void set_timeout(struct timespec *ts, t_fixed64 timeout_ms);
 
#endif