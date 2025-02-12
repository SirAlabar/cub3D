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
# include <types.h>
# include <stddef.h>
# include <libft.h>

/* BSP Configuration */
# define BSP_WEIGHT_FACTOR     3
# define BSP_MIN_SEED         0
# define BSP_MAX_SEED         1000
# define BSP_MAX_DEPTH        16
# define BSP_MIN_NODE_SIZE    4096 

/* Fixed point value for collision detection (1/16) */
# define COLLISION_THRESHOLD   4096
/* Penalty factor for each line split during partition */
# define SPLIT_PENALTY        8

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
    int                 depth;	
}						t_bsp_node;

/*
 Main BSP tree structure
 Contains root node and lookup tables for trigonometric functions
 The tables provide fast access to pre-calculated values
*/
typedef struct s_bsp_tree
{
	t_bsp_node			*root;
    t_fixed32           best_score;
    unsigned int        best_seed;
    int                 max_depth; 	
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
	int					total_splits;
	int					tree_depth;
	int					max_depth;	
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
t_bsp_node	*build_bsp_tree(t_bsp_line **lines, int num_lines, int depth);
t_bsp_node	*build_subtrees(t_bsp_node *node, t_bsp_data *data);
t_bsp_line				*choose_partition(t_bsp_line **lines, int num_lines);
t_fixed32	eval_partition(t_bsp_line *partition, t_bsp_line **lines,
		int num_lines, int depth);
		void	count_line_sides(t_bsp_line *line, t_bsp_line *partition,
		t_count_data *count);

/* bsp_classify.c */
t_bsp_side				bsp_classify_line(t_bsp_line *line,
							t_bsp_line *partition);
t_bsp_side				bsp_classify_point(t_fixed_vec32 point,
							t_bsp_line *partition);

/* bsp_create_split_lines.c */
bool					split_bsp_line(t_bsp_line *line, t_bsp_line *partition,
							t_bsp_line **front, t_bsp_line **back);

/* bsp_split_lines.c */
bool					split_lines(t_bsp_line *partition, t_bsp_line **lines,
							int num_lines, t_bsp_data *data);

/* bsp_traverse.c */
t_bsp_node	*find_node(t_bsp_node *node, t_fixed_vec32 point);
void	traverse_front_to_back(t_bsp_node *node, t_fixed_vec32 viewpoint,
		void (*process_node)(t_bsp_node *));
void	traverse_front_nodes(t_bsp_node *node, t_fixed_vec32 viewpoint,
		void (*process_node)(t_bsp_node *));
int	count_front_nodes(t_bsp_node *node, t_fixed_vec32 viewpoint);

/* bsp_utils.c */
t_bsp_tree				*init_bsp_build(t_doom_map *map);
void					*free_and_return(void *ptr, void *ret);
t_bsp_node				*create_bsp_node(void);
t_bsp_line				*create_bsp_line(t_fixed_vec32 start, t_fixed_vec32 end,
							int type);
bool					extract_map_lines(t_doom_map *map, t_bsp_line ***lines,
							int *num_lines);

/* bsp_balance.c */
bool        balance_bsp_tree(t_bsp_tree *tree);

/* bsp_intersect.c */
t_fixed32   find_nearest_wall(t_bsp_node *node, t_fixed_vec32 point);
bool        check_movement_valid(t_bsp_tree *tree, t_fixed_vec32 start,
                t_fixed_vec32 end);
void        adjust_collision_response(t_bsp_node *node, t_fixed_vec32 *movement);

/* bsp_validate.c */
bool    validate_bsp_tree(t_bsp_tree *tree);
void    debug_print_node(t_bsp_node *node, int depth);

void            free_bsp_tree(t_bsp_tree *tree);
void	print_bsp_tree(t_bsp_tree *tree);
void	print_bsp_tree_recursive(t_bsp_node *node, int depth, char *prefix);

void	count_line_sides(t_bsp_line *line, t_bsp_line *partition,
		t_count_data *count);
t_bsp_node	*build_subtrees(t_bsp_node *node, t_bsp_data *data);


unsigned int	generate_random_seed(void);
void	shuffle_lines(t_bsp_line **lines, int count, unsigned int seed);
t_fixed32	evaluate_seed_quality(t_bsp_line **lines, int count,
		unsigned int seed, int depth);
unsigned int	find_best_seed(t_bsp_line **lines, int count, int depth);		
void	init_count_data(t_count_data *count);

t_bsp_node	*init_node(t_bsp_line **lines, int num_lines);
void	free_bsp_node(t_bsp_node *node);

#endif
