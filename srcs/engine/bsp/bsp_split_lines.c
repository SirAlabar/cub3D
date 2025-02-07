/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bsp.h>

/*
** Initializes arrays for split operation
** Returns false if allocation fails
*/
static bool	init_split_arrays(t_bsp_data *data, int num_lines)
{
	data->front_lines = malloc(sizeof(t_bsp_line *) * num_lines * 2);
	data->back_lines = malloc(sizeof(t_bsp_line *) * num_lines * 2);
	if (!data->front_lines || !data->back_lines)
	{
		free(data->front_lines);
		free(data->back_lines);
		return (false);
	}
	data->num_front = 0;
	data->num_back = 0;
	return (true);
}

/*
** Handles line addition after classification or splitting
** Updates front and back line counts
*/
static void	add_line_to_side(t_bsp_data *data, t_bsp_line *line,
		t_bsp_side side)
{
	if (side == BSP_FRONT)
	{
		data->front_lines[data->num_front] = line;
		data->num_front++;
	}
	else if (side == BSP_BACK)
	{
		data->back_lines[data->num_back] = line;
		data->num_back++;
	}
}

/*
** Processes a single line during split operation
** Either adds the line to a side or splits it if spanning
*/
static bool process_line(t_bsp_data *data, t_bsp_line *line,
        t_bsp_line *partition)
{
    t_bsp_side  side;
    t_bsp_line  *front_split = NULL;
    t_bsp_line  *back_split = NULL;ma

    side = bsp_classify_line(line, partition);

    switch (side) {
        case BSP_FRONT:
        case BSP_BACK:
            add_line_to_side(data, line, side);
            break;
        case BSP_SPANNING:
            // Tentar split, mas não abortar se falhar
            if (split_bsp_line(line, partition, &front_split, &back_split)) {
                if (front_split)
                    add_line_to_side(data, front_split, BSP_FRONT);
                if (back_split)
                    add_line_to_side(data, back_split, BSP_BACK);
            } else {
                // Se split falhar, tenta classificar de forma mais tolerante
                if (bsp_classify_point(line->start, partition) == BSP_FRONT ||
                    bsp_classify_point(line->end, partition) == BSP_FRONT)
                    add_line_to_side(data, line, BSP_FRONT);
                else
                    add_line_to_side(data, line, BSP_BACK);
            }
            break;
        default:
            // Caso colinear ou outros
            add_line_to_side(data, line, BSP_FRONT);
            break;
    }

    return true;
}

/*
** Splits a set of lines into front and back groups
** based on their position relative to partition
*/
bool	split_lines(t_bsp_line *partition, t_bsp_line **lines, int num_lines,
		t_bsp_data *data)
{
	int	i;

	if (!init_split_arrays(data, num_lines))
		return (false);
	i = 0;
	while (i < num_lines)
	{
		if (lines[i] != partition)
		{
			if (!process_line(data, lines[i], partition))
				return (false);
		}
		i++;
	}
	return (true);
}
