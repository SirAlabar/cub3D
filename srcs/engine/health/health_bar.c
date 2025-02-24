/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_bar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:37:30 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 18:01:51 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <cub3d.h>

static t_bar	init_bar(void)
{
	t_bar	bar;
	bar.width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + 20;
	bar.height = 40;
	bar.start_x = MINIMAP_PADDING + 770; // This positions it at the corner
	bar.start_y = WINDOW_HEIGHT - MINIMAP_PADDING - bar.height - 10;
	return (bar);
}

void	draw_lifebar_hud(t_game *game, t_texture *image)
{
	t_bar	bar;
	int		color;
	int		i;
	int		j;
	int		x_offset;
	int		y_offset;
	
	bar = init_bar();
	
	// Calculate offsets to position the HUD image correctly relative to the bar
	x_offset = bar.start_x + bar.width - image->width + 4;
	y_offset = bar.start_y - image->height + bar.height;
	
	i = -1;
	while (++i < image->height)
	{
		j = -1;
		while (++j < image->width)
		{
			color = get_texture_pixel(image, j, i);
			if (color != 0xFFC0CB)
				draw_pixel(game, x_offset + j, y_offset + i, color);
		}
	}
}

static void	draw_health_bar_background(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;
	
	bar = init_bar();
	bar.color = 0x555555;
	bar.width += 0;
	bar.height += 0;
	
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < bar.width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i, bar.color);
	}
}

static void	draw_health_bar_fill(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;
	int		fill_width;
	
	bar = init_bar();
	fill_width = (bar.width * game->p1.health) / 100;
	
	// Set color based on health percentage
	if (game->p1.health > 70)
		bar.color = 0x00FF00; // Green for high health
	else if (game->p1.health > 30)
		bar.color = 0xFFFF00; // Yellow for medium health
	else
		bar.color = 0xFF0000; // Red for low health
	
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < fill_width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i, bar.color);
	}
}

void	draw_health_bar(t_game *game)
{
	t_texture	*health;
	
	health = texture_create(game, "./assets/sprites/health/healthbar.xpm");
	if (!health)
	{
		printf("Error\nFalha ao carregar a textura do HUD!\n");
		cleanup_game(game);
		exit(1);
	}
	draw_health_bar_background(game);
	draw_health_bar_fill(game);
	draw_lifebar_hud(game, health);
	
	// Clean up the texture to prevent memory leaks
	texture_destroy(&health, game->mlx);
}
