/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_bar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 10:37:30 by marsoare          #+#    #+#             */
/*   Updated: 2025/02/24 17:45:51 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_bar	init_bar(void)
{
	t_bar	bar;

	bar.width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE;
	bar.height = 20;
	bar.start_x = MINIMAP_PADDING;
	bar.start_y = WINDOW_HEIGHT - MINIMAP_PADDING - bar.height;
	return (bar);
}

void	draw_lifebar_hud(t_game *game, t_texture *image)
{
	t_bar	bar;
	int		color;
	int		i;
	int		j;

	bar = init_bar();
	i = -1;
	while (++i < image->height)
	{
		j = -1;
		while (++j < image->width)
		{
			color = get_texture_pixel(image, j, i);
			if (color != 0xFFC0CB)
				draw_pixel(game, (WINDOW_WIDTH - image->width) - 30 + j,
					bar.start_y - image->height + 30 + i, color);
		}
	}
}

static void	draw_health_bar_background(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;

	bar = init_bar();
	bar.color = 0xFF3000;
	bar.start_y -= 40;
	bar.start_x += 775;
	bar.width += 20;
	bar.height += 16;
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < bar.width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i + 30, bar.color);
	}
}

static void	draw_health_bar_fill(t_game *game)
{
	t_bar	bar;
	int		i;
	int		j;
	int		fill_width;

	bar = init_bar();
	// Calculate fill width based on player's current health percentage
	fill_width = (bar.width * game->p1.health) / 100;
	
	// Use original colors based on your code
	if (game->p1.health > 50)
		bar.color = 0xFF0000;
	else
		bar.color = 0x00FF00;
	
	i = -1;
	while (++i < bar.height)
	{
		j = -1;
		while (++j < fill_width)
			draw_pixel(game, bar.start_x + j, bar.start_y + i + 30, bar.color);
	}
}

void	draw_health_bar(t_game *game)
{
	t_texture	*health;

	(health = texture_create(game, "./assets/sprites/health/healthbar.xpm"));
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
