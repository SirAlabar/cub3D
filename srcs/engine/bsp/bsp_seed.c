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
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

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

static void	swap_lines(t_bsp_line **lines, int i, int j)
{
	t_bsp_line	*temp;

	temp = lines[i];
	lines[i] = lines[j];
	lines[j] = temp;
}

void	shuffle_lines(t_bsp_line **lines, int count, unsigned int seed)
{
	int				i;
	unsigned int	j;

	if (!lines || count <= 1)
		return ;
	srand(seed);
	i = count - 1;
	while (i > 0)
	{
		j = (unsigned int)rand() % (i + 1);
		swap_lines(lines, i, j);
		i--;
	}
}


static t_fixed32	eval_seed_quality(unsigned int seed, t_bsp_line **lines,
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
	score = int_to_fixed32(abs(count.back - count.front) + 
	count.split * SPLIT_PENALTY);
	free(test_lines);
	return (score);
}

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
	return (NULL);
}

static unsigned int	get_best_seed_from_threads(t_seed_data *thread_data,
	int num_threads)
{
	int				i;
	unsigned int	best_seed;
	t_fixed32		best_score;

	best_score = INT32_MAX;
	best_seed = BSP_MIN_SEED;
	i = -1;
	while (++i < num_threads)
	{
		if (thread_data[i].best_score < best_score)
		{
			best_score = thread_data[i].best_score;
			best_seed = thread_data[i].best_seed;
		}
	}
	return (best_seed);
}

unsigned int	find_best_seed(t_bsp_line **lines, int count, int depth)
{
	pthread_t		threads[NUM_THREADS];
	t_seed_data		thread_data[NUM_THREADS];
	int				i;
	unsigned int	range;
	pthread_mutex_t	mutex;

	if (depth > 0)
		return (generate_random_seed());
	pthread_mutex_init(&mutex, NULL);
	range = (BSP_MAX_SEED - BSP_MIN_SEED) / NUM_THREADS;
	i = -1;
	while (++i < NUM_THREADS)
	{
		thread_data[i].start_seed = BSP_MIN_SEED + i * range;
		thread_data[i].end_seed = (i == NUM_THREADS - 1) ? 
			BSP_MAX_SEED : BSP_MIN_SEED + (i + 1) * range;
		thread_data[i].lines = lines;
		thread_data[i].num_lines = count;
		thread_data[i].best_seed = thread_data[i].start_seed;
		thread_data[i].best_score = INT32_MAX;
		thread_data[i].thread_id = i;
		thread_data[i].mutex = &mutex;
		pthread_create(&threads[i], NULL, find_seed_thread, &thread_data[i]);
	}
	i = -1;
	while (++i < NUM_THREADS)
		pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&mutex);
	ft_printf("Busca por seed concluída em %d threads\n", NUM_THREADS);
	return (get_best_seed_from_threads(thread_data, NUM_THREADS));
}