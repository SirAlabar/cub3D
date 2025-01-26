/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/16 20:26:37 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <minimap.h>

static bool	is_valid_map_position(t_game *game, int x, int y)
{
	if (x < 0 || y < 0 || x >= game->map.height || y >= game->map.width)
		return (false);
	if (y >= (int)ft_strlen(game->map.grid[x]))
		return (false);
	return (true);
}

static void	draw_map_cell(t_game *game, int screen_x, int screen_y,
		t_minimap_data *data)
{
	int	map_x;
	int	map_y;

	map_x = data->player_map_x + (screen_y - data->view_radius);
	map_y = data->player_map_y + (screen_x - data->view_radius);
	if (!is_valid_map_position(game, map_x, map_y))
		return ;
	if (game->map.grid[map_x][map_y] == '1')
		draw_minimap_cell(game, screen_x, screen_y, 0xFFFFFF);
	else if (game->map.grid[map_x][map_y] == '0'
		|| ft_strchr("NSEW", game->map.grid[map_x][map_y]))
		draw_minimap_cell(game, screen_x, screen_y, 0x808080);
}

static void	draw_map_grid(t_game *game, t_minimap_data *data)
{
	int	screen_y;
	int	screen_x;

	screen_y = 0;
	while (screen_y < MINIMAP_VIEW_SIZE)
	{
		screen_x = 0;
		while (screen_x < MINIMAP_VIEW_SIZE)
		{
			draw_map_cell(game, screen_x, screen_y, data);
			screen_x++;
		}
		screen_y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_minimap_data	data;

	data.player_map_x = (int)game->p1.pos.x;
	data.player_map_y = (int)game->p1.pos.y;
	data.view_radius = MINIMAP_VIEW_SIZE / 2;
	draw_minimap_background(game);
	draw_map_grid(game, &data);
	draw_player_dot(game);
}
