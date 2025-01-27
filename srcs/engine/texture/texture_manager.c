/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:45:06 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:22:39 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_texture	*texture_create(t_game *game, char *texture_path)
{
	t_texture	*texture;

	texture = (t_texture *)ft_calloc(1, sizeof(t_texture));
	if (!texture)
	{
		ft_printf("Error: Failed to allocate memory for texture\n");
		return (NULL);
	}
	texture->img = mlx_xpm_file_to_image(game->mlx, texture_path,
			&texture->width, &texture->height);
	if (!texture->img)
	{
		ft_printf("Error: Failed to load texture: %s\n", texture_path);
		free(texture);
		return (NULL);
	}
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
			&texture->line_len, &texture->endian);
	return (texture);
}

t_texture	*get_wall_texture(t_ray *ray, t_game *game)
{
	if (ray->is_door)
		return (&game->door_system->door_texture);
	if (ray->side == 0)
	{
		if (ray->dir.x > 0)
			return (&game->west);
		else
			return (&game->east);
	}
	else
	{
		if (ray->dir.y > 0)
			return (&game->north);
		else
			return (&game->south);
	}
}

static t_vector_i	get_scale_coords(t_vector_i pos, double scale_x,
		double scale_y)
{
	t_vector_i	src;

	src.x = pos.x * scale_x;
	src.y = pos.y * scale_y;
	return (src);
}

static double	get_scale_factor(int src_size, int dst_size)
{
	return ((double)src_size / dst_size);
}

void	resize_texture(t_texture *src, t_texture *dst)
{
	t_vector_i	pos;
	t_vector_i	src_pos;
	double		scale_x;
	double		scale_y;
	int			color;

	scale_x = get_scale_factor(src->width, dst->width);
	scale_y = get_scale_factor(src->height, dst->height);
	pos.y = -1;
	while (++pos.y < dst->height)
	{
		pos.x = -1;
		while (++pos.x < dst->width)
		{
			src_pos = get_scale_coords(pos, scale_x, scale_y);
			color = get_texture_pixel(src, src_pos.x, src_pos.y);
			if ((color & 0xFFC0CB) == 0)
				continue ;
			draw_texture_pixel(dst, pos.x, pos.y, color);
		}
	}
}
