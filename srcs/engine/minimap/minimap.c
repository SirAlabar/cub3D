/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:20:40 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/22 20:10:09 by hluiz-ma         ###   ########.fr       */
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

	map_x = data->player_map_x + (screen_x - data->view_radius);
	map_y = data->player_map_y + (screen_y - data->view_radius);
	if (!is_valid_map_position(game, map_y, map_x))
		return ;
	if (game->map.grid[map_x][map_y] == '1')
		draw_minimap_cell(game, screen_x, screen_y, 0xFFFFFF);
	else if (game->map.grid[map_x][map_y] == 'D')
		draw_minimap_cell(game, screen_x, screen_y, 0xAAAA00);
	else if (game->map.grid[map_x][map_y] == '0' || ft_strchr("NSEW",
			game->map.grid[map_x][map_y]))
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

void	draw_minihud(t_game *game, t_texture *image)
{
	t_vector_i	pos;
	t_vector_i	img_pos;
	int			color;
	int			height;

	height = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING);
	img_pos.x = MINIMAP_PADDING;
	img_pos.y = WINDOW_HEIGHT - height;
	pos.y = -1;
	while (++pos.y < image->height)
	{
		pos.x = -1;
		while (++pos.x < image->width)
		{
			color = get_texture_pixel(image, pos.x, pos.y);
			if (color != 0xFFC0CB)
				draw_pixel(game, img_pos.x + pos.x, img_pos.y + pos.y, color);
		}
	}
}

void	draw_minimap(t_game *game)
{
	t_minimap_data	data;

	(game->minihud = texture_create(game, MINIHUD));
	(game->minihudbg = texture_create(game, MINIHUDBG));
	if (!game->minihud)
	{
		ft_printf("Error\nFailed to load the HUD texture!\n");
		cleanup_game(game);
		exit(1);
	}
	data.player_map_x = (int)game->p1.pos.x;
	data.player_map_y = (int)game->p1.pos.y;
	data.view_radius = MINIMAP_VIEW_SIZE >> 1;
	draw_minihud(game, game->minihudbg);
	draw_minimap_background(game);
	draw_map_grid(game, &data);
	draw_player_dot(game);
	draw_minihud(game, game->minihud);
	texture_destroy(&game->minihud, game->mlx);
	texture_destroy(&game->minihudbg, game->mlx);
}
