/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_seed.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
 * Global seed tracking structure
 */
typedef struct s_global_best
{
	pthread_mutex_t	mutex;
	t_fixed32		best_score;
	unsigned int	best_seed;
}	t_global_best;

unsigned int	generate_random_seed(void)
{
	static int	initialized = 0;

	if (!initialized)
	{
		srand(time(NULL) * getpid());
		initialized = 1;
	}
	return ((unsigned int)rand() % (BSP_MAX_SEED - BSP_MIN_SEED + 1) 
		+ BSP_MIN_SEED);
}

/*
 * Swaps two lines in the array
 */
void	shuffle_lines(t_bsp_line **lines, int count, unsigned int seed)
{
	int				i;
	unsigned int	j;
	t_bsp_line		*temp;

	if (!lines || count <= 1)
		return ;
	srand(seed);
	i = count - 1;
	while (i > 0)
	{
		j = (unsigned int)rand() % (i + 1);
		temp = lines[i];
		lines[i] = lines[j];
		lines[j] = temp;
		i--;
	}
}

/*
 * Initialize a seed task with the given parameters
 */
static void	init_seed_task(t_seed_data *task, t_bsp_line **lines,
	int count, int index, unsigned int range, t_global_best *global)
{
	task->start_seed = BSP_MIN_SEED + index * range;
	if (index == THREAD_COUNT - 1)
		task->end_seed = BSP_MAX_SEED;
	else
		task->end_seed = BSP_MIN_SEED + (index + 1) * range;
	task->lines = lines;
	task->num_lines = count;
	task->best_seed = task->start_seed;
	task->best_score = INT32_MAX;
	task->thread_id = index;
	task->mutex = &global->mutex;
}

/*
 * Evaluates a single seed's quality
 */
t_fixed32	eval_seed_quality(unsigned int seed, t_bsp_line **lines,
	int num_lines)
{
	t_bsp_line		**test_lines;
	t_bsp_line		*partition;
	t_count_data	count;
	t_fixed32		score;
	int				i;

	init_count_data(&count);
	test_lines = ft_calloc(num_lines, sizeof(t_bsp_line *));
	if (!test_lines)
		return (INT32_MAX);
	i = -1;
	while (++i < num_lines)
		test_lines[i] = lines[i];
	shuffle_lines(test_lines, num_lines, seed);
	partition = test_lines[0];
	i = -1;
	while (++i < num_lines)
	{
		if (test_lines[i] != partition)
			count_line_sides(test_lines[i], partition, &count);
	}
	score = int_to_fixed32(abs(count.back - count.front)
			+ count.split * SPLIT_PENALTY);
	free(test_lines);
	return (score);
}

/*
 * Worker function for evaluating a range of seeds
 */
static void	*find_seed_thread(void *arg)
{
	t_seed_data		*data;
	unsigned int	seed;
	t_fixed32		score;
	t_fixed32		best_score;
	unsigned int	best_seed;

	data = (t_seed_data *)arg;
	best_score = INT32_MAX;
	best_seed = data->start_seed;
	seed = data->start_seed - 1;
	while (++seed < data->end_seed)
	{
		score = eval_seed_quality(seed, data->lines, data->num_lines);
		if (score < best_score)
		{
			best_score = score;
			best_seed = seed;
		}
	}
	data->best_score = best_score;
	data->best_seed = best_seed;
	return (data);
}

/*
 * Callback when a seed evaluation task completes
 */
static void	seed_task_complete(void *result, void *context)
{
	t_seed_data		*task_result;
	t_global_best	*global;

	task_result = (t_seed_data *)result;
	global = (t_global_best *)context;
	pthread_mutex_lock(&global->mutex);
	if (task_result->best_score < global->best_score)
	{
		global->best_score = task_result->best_score;
		global->best_seed = task_result->best_seed;
	}
	pthread_mutex_unlock(&global->mutex);
}

/*
 * Finds the best seed for BSP partitioning using thread pool
 */
unsigned int	find_best_seed(t_bsp_line **lines, int count, int depth)
{
	t_thread_pool	*pool;
	t_seed_data		*tasks;
	t_global_best	global;
	int				i;
	unsigned int	range;

	if (depth > 0)
		return (generate_random_seed());
	pthread_mutex_init(&global.mutex, NULL);
	global.best_score = INT32_MAX;
	global.best_seed = BSP_MIN_SEED;
	pool = thread_pool_create(THREAD_COUNT);
	if (!pool)
		return (generate_random_seed());
	tasks = ft_calloc(THREAD_COUNT, sizeof(t_seed_data));
	if (!tasks)
		return (thread_pool_destroy(pool), generate_random_seed());
	range = (BSP_MAX_SEED - BSP_MIN_SEED) / THREAD_COUNT;
	i = -1;
	while (++i < THREAD_COUNT)
	{
		init_seed_task(&tasks[i], lines, count, i, range, &global);
		thread_pool_add_task(pool, find_seed_thread, &tasks[i],
			seed_task_complete, &global);
	}
	thread_pool_wait(pool);
	thread_pool_destroy(pool);
	pthread_mutex_destroy(&global.mutex);
	free(tasks);
	ft_printf("Seed search completed using %d threads\n", THREAD_COUNT);
	return (global.best_seed);
}