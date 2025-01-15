/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/14 22:00:38 by marvin           ###   ########.fr       */
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

void resize_texture(t_texture *src, t_texture *dst)
{
    int x, y;
    unsigned int color;
    double src_x, src_y;
    double scale_x = (double)src->width / dst->width;
    double scale_y = (double)src->height / dst->height;
    
    y = -1;
    while (++y < dst->height)
    {
        x = -1;
        while (++x < dst->width)
        {
            src_x = x * scale_x;
            src_y = y * scale_y;
            color = get_texture_pixel(src, (int)src_x, (int)src_y);
            if ((color & 0xFFC0CB) == 0)
                continue;
            draw_texture_pixel(dst, x, y, color);
        }
    }
}

void	texture_destroy(t_texture **texture, void *mlx)
{
	if (*texture)
	{
		if ((*texture)->img)
		{
			mlx_destroy_image(mlx, (*texture)->img);
			(*texture)->img = NULL;
		}
		free(*texture);
		*texture = NULL;
	}
}
