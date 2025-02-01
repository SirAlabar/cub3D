/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/01 18:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H

# include <fixed_point.h>

/*
 Structure representing a line/segment in the BSP tree
 Uses fixed-point arithmetic for precise calculations
 The normal vector is pre-calculated for efficiency
*/
typedef struct s_bsp_line
{
	t_fixed_vec32		start;
	t_fixed_vec32		end;
	t_fixed_vec32		normal;
	int					type;
}						t_bsp_line;

/*
 Node structure for the BSP tree
 Each node contains a partition line that divides space
 Front child contains space in front of partition
 Back child contains space behind partition
*/
typedef struct s_bsp_node
{
	t_bsp_line			*partition;
	struct s_bsp_node	*front;
	struct s_bsp_node	*back;
	t_bsp_line			**lines;
	int					num_lines;
}						t_bsp_node;

/*
 Main BSP tree structure
 Contains root node and lookup tables for trigonometric functions
 The tables provide fast access to pre-calculated values
*/
typedef struct s_bsp_tree
{
	t_bsp_node			*root;
}						t_bsp_tree;

/*
 Helper structure for splitting lines
 Used to keep function parameters within norm limits
*/
typedef struct s_bsp_data
{
	t_bsp_line			**front_lines;
	t_bsp_line			**back_lines;
	int					num_front;
	int					num_back;
}						t_bsp_data;

/*
** Structure for BSP partition quality metrics
** Used during partition line selection to create a balanced tree
** - front: Counts lines in front of potential partition
** - back:  Counts lines behind potential partition
** - split: Counts lines that would be split by partition
** Lower split count and balanced front/back ratio indicates better partition
** This helps optimize BSP tree for efficient rendering and collision detection
*/
typedef struct s_count_data
{
	int					front;
	int					back;
	int					split;
}						t_count_data;

/*
 Enumeration for classification of points/lines relative to partition
 Used during tree construction and traversal
 - FRONT: Point/line is in front of partition
 - BACK: Point/line is behind partition
 - SPANNING: Line crosses partition
 - COLINEAR: Line lies on partition
*/
typedef enum e_bsp_side
{
	BSP_FRONT = 1,
	BSP_BACK = -1,
	BSP_SPANNING = 0,
	BSP_COLINEAR = 2
}						t_bsp_side;

/*
** Function prototypes
*/

/* bsp_build.c */
t_bsp_node				*build_bsp_tree(t_bsp_line **lines, int num_lines);
t_bsp_line				*choose_partition(t_bsp_line **lines, int num_lines);
t_fixed32				eval_partition(t_bsp_line *partition,
							t_bsp_line **lines, int num_lines);

/* bsp_create_split_lines.c */
bool					split_bsp_line(t_bsp_line *line, t_bsp_line *partition,
							t_bsp_line **front, t_bsp_line **back);

/* bsp_split_lines.c */
bool					split_lines(t_bsp_line *partition, t_bsp_line **lines,
							int num_lines, t_bsp_data *data);

/* bsp_utils.c */
t_bsp_tree				*init_bsp_build(t_game *game);
void					*free_and_return(void *ptr, void *ret);
t_bsp_node				*create_bsp_node(void);
bool					extract_map_lines(t_game *game, t_bsp_line ***lines,
							int *num_lines);

/* bsp_classify.c */
t_bsp_side				bsp_classify_line(t_bsp_line *line,
							t_bsp_line *partition);
t_bsp_side				bsp_classify_point(t_fixed_vec32 point,
							t_bsp_line *partition);

#endif
