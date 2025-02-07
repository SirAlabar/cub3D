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


t_fixed32	evaluate_seed(t_bsp_line **lines, int count, 
						unsigned int seed, int depth)
{
	t_bsp_line		**test_lines;
	t_fixed32		total_score;
	int				i;
	int				test_count;

	test_lines = ft_calloc(count, sizeof(t_bsp_line *));
	if (!test_lines)
		return (INT32_MAX);
	ft_memcpy(test_lines, lines, sizeof(t_bsp_line *) * count);
	shuffle_lines(test_lines, count, seed);
	total_score = 0;
	test_count = (count < 5) ? count : 5;
	i = 0;
	while (i < test_count)
	{
		total_score = fixed32_add(total_score,
				eval_partition(test_lines[i], lines, count, depth));
		i++;
	}
	free(test_lines);
	return (fixed32_div(total_score, int_to_fixed32(test_count)));
}

unsigned int	find_best_seed(t_bsp_line **lines, int count, int depth)
{
	unsigned int	best_seed;
	t_fixed32		best_score;
	int				attempts;
	unsigned int	current_seed;
	t_fixed32		current_score;

	best_seed = generate_random_seed();
	best_score = evaluate_seed(lines, count, best_seed, depth);
	attempts = 10;
	while (attempts-- > 0)
	{
		current_seed = generate_random_seed();
		current_score = evaluate_seed(lines, count, current_seed, depth);
		if (current_score < best_score)
		{
			best_score = current_score;
			best_seed = current_seed;
		}
	}
	return (best_seed);
}