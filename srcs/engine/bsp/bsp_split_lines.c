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
#include <colors.h>

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
		ft_printf(RED"Failed to allocate memory for split arrays\n"DEFAULT);		
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
static void add_line_to_side(t_bsp_data *data, t_bsp_line *line, t_bsp_side side)
{
	if (side == BSP_FRONT || side == BSP_COLINEAR)
	{
		data->front_lines[data->num_front] = line;
		data->num_front++;
		ft_printf(GREEN"Added line to FRONT side (count: %d)\n"DEFAULT, data->num_front);
	}
	else if (side == BSP_BACK)
	{
		data->back_lines[data->num_back] = line;
		data->num_back++;
		ft_printf(GREEN"Added line to BACK side (count: %d)\n"DEFAULT, data->num_back);
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
    t_bsp_line  *back_split = NULL;

    side = bsp_classify_line(line, partition);
	ft_printf(BLUE"Classifying line: (%d,%d) -> (%d,%d) = %d\n"DEFAULT,
		fixed32_to_int(line->start.x), fixed32_to_int(line->start.y),
		fixed32_to_int(line->end.x), fixed32_to_int(line->end.y),
		side);
    if (side == BSP_FRONT || side == BSP_BACK || side == BSP_COLINEAR)
    {
        add_line_to_side(data, line, side);
        return (true);
    }
    else if (side == BSP_SPANNING)
    {
        if (split_bsp_line(line, partition, &front_split, &back_split))
        {
            if (front_split)
                add_line_to_side(data, front_split, BSP_FRONT);
            if (back_split)
                add_line_to_side(data, back_split, BSP_BACK);
            return (true);
        }
        else
        {
            if (bsp_classify_point(line->start, partition) == BSP_FRONT)
                add_line_to_side(data, line, BSP_FRONT);
            else
                add_line_to_side(data, line, BSP_BACK);
            return (true);
        }
    }
    return (false);
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
