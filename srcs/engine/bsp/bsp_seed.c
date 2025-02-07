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

/*
** Generates random seed value using rand()
** Seeds with time if not initialized
*/
unsigned int	generate_random_seed(void)
{
	static int	initialized = 0;

	if (!initialized)
	{
		srand(time(NULL));
		initialized = 1;
	}
	return ((unsigned int)rand() % (BSP_MAX_SEED - BSP_MIN_SEED + 1) 
		+ BSP_MIN_SEED);
}

/*
** Swaps two BSP lines positions in array
*/
static void	swap_lines(t_bsp_line **lines, int i, int j)
{
	t_bsp_line	*temp;

	temp = lines[i];
	lines[i] = lines[j];
	lines[j] = temp;
}

/*
** Shuffles BSP lines array using Fisher-Yates algorithm
*/
void	shuffle_lines(t_bsp_line **lines, int count, unsigned int seed)
{
	int				i;
	unsigned int	j;

	if (!lines)
		return ;
	if (count <= 1)
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

/*
** Evaluates quality of a specific seed for partitioning
** Tests multiple partitions and calculates average score
*/
t_fixed32	evaluate_seed_quality(t_bsp_line **lines, int count,
		unsigned int seed, int depth)
{
	t_bsp_line	**test_lines;
	t_fixed32	total_score;
	int			i;
	int			test_count;

	test_lines = ft_calloc(count, sizeof(t_bsp_line *));
	if (!test_lines)
		return (INT32_MAX);
	ft_memcpy(test_lines, lines, sizeof(t_bsp_line *) * count);
	shuffle_lines(test_lines, count, seed);
	total_score = 0;
	if (count < 5)
		test_count = count;
	else
		test_count = 5;
	i = 0;
	while (i < test_count)
	{
		total_score = fixed32_add(total_score,
				eval_partition(test_lines[i], test_lines, count, depth));
		i++;
	}
	free(test_lines);
	return (fixed32_div(total_score, int_to_fixed32(test_count)));
}

/*
** Finds best seed by testing multiple options
*/
unsigned int	find_best_seed(t_bsp_line **lines, int count, int depth)
{
	unsigned int	current_seed;
	unsigned int	best_seed;
	t_fixed32		current_score;
	t_fixed32		best_score;
	int				test_seeds;

	if (!lines)
		return (BSP_MIN_SEED);
	if (count <= 1)
		return (BSP_MIN_SEED);
	best_seed = generate_random_seed();
	best_score = evaluate_seed_quality(lines, count, best_seed, depth);
	test_seeds = 10;
	while (test_seeds > 0)
	{
		current_seed = generate_random_seed();
		current_score = evaluate_seed_quality(lines, count, current_seed, depth);
		if (current_score < best_score)
		{
			best_score = current_score;
			best_seed = current_seed;
		}
		test_seeds--;
	}
	return (best_seed);
}
