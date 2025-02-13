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

unsigned int	apply_shade(unsigned int color, double shade)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = ((color >> 16) & 0xFF) * shade;
	g = ((color >> 8) & 0xFF) * shade;
	b = (color & 0xFF) * shade;
	return ((0xFF << 24) | (r << 16) | (g << 8) | b);
}

unsigned int	get_texture_pixel(t_texture *tex, int x, int y)
{
	return (*(unsigned int *)(tex->addr + (y * tex->line_len + x * (tex->bpp
				/ 8))));
}

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

/*
t_texture *get_wall_texture(t_ray *ray, t_game *game)
{
    t_linedef *linedef;
    t_sidedef *sidedef;
    int sidedef_index;


    if (ray->hit_info.linedef < 0 || ray->hit_info.linedef >= game->map->linedef_count)
        return NULL;

    linedef = &game->map->linedefs[ray->hit_info.linedef];
    sidedef_index = linedef->front_sidedef;
    if (sidedef_index < 0 || sidedef_index >= game->map->sidedef_count)
        return NULL;

    sidedef = &game->map->sidedefs[sidedef_index];
    return texture_create(game, sidedef->middle_texture);
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
*/