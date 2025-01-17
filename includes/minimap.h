/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/16 14:20:40 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# define MINIMAP_CELL_SIZE 12
# define MINIMAP_PADDING 20
# define PLAYER_DOT_SIZE 6
# define MINIMAP_VIEW_SIZE 11

typedef struct s_minimap_data
{
	int	player_map_x;
	int	player_map_y;
	int	view_radius;
}	t_minimap_data;

// minimap.c
void	draw_minimap(t_game *game);

// minimap_draw.c
void	draw_minimap_cell(t_game *game, int screen_x, int screen_y, int color);
void	draw_direction_line(t_game *game, t_vector center,
			t_vector dir, int length);

// minimap_player.c
void	draw_player_dot(t_game *game);

// minimap_background.c
void	draw_minimap_background(t_game *game);

#endif
