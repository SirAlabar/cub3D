/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health_bar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:58:24 by marsoare          #+#    #+#             */
/*   Updated: 2025/03/09 12:58:57 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_bar	init_bar(void)
{
	t_bar	bar;

	bar.width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + 20;
	bar.height = 40;
	bar.start_x = MINIMAP_PADDING + 770;
	bar.start_y = WINDOW_HEIGHT - MINIMAP_PADDING - bar.height - 10;
	bar.color = 0;
	return (bar);
}

void	draw_lifebar_hud(t_game *game, t_texture *image)
{
	t_bar		bar;
	unsigned int			color;
	int			i;
	int			j;
	t_vector_i	offset;

	if (!game || !image)
		return ;
	bar = init_bar();
	offset.x = bar.start_x + bar.width - image->width + 4;
	offset.y = bar.start_y - image->height + bar.height;
	i = -1;
	while (++i < image->height)
	{
		j = -1;
		while (++j < image->width)
		{
			if (j >= 0 && j < image->width && i >= 0 && i < image->height)
			{
				color = get_texture_pixel(image, j, i);
				if (color != 0xFFC0CB)
					draw_pixel(game, offset.x + j, offset.y + i, color);
			}
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
	if (game->p1.health > 70)
		bar.color = 0x00FF00;
	else if (game->p1.health > 30)
		bar.color = 0xFFFF00;
	else
		bar.color = 0xFF0000;
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

	if (!game)
		return ;
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
	texture_destroy(&health, game->mlx);
}
