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
	int			offset;
	int			width;
	int			height;

	if (!tex || !tex->addr || x < 0 || y < 0 || x >= tex->width || y >= tex->height)
	{
		width = 0;
		height = 0;
		if (tex)
		{
			width = tex->width;
			height = tex->height;
		}
		ft_printf("Invalid texture access: x=%d, y=%d (dim: %dx%d)\n",
			x, y, width, height);
		return (0);
	}
	offset = (y * tex->line_len + x * (tex->bpp / 8));
	if (offset < 0 || offset >= (tex->height * tex->line_len))
	{
		ft_printf("Invalid texture offset: %d (max: %d)\n",
			offset, tex->height * tex->line_len);
		return (0);
	}
	return (*(unsigned int *)(tex->addr + offset));
}
/*
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
}*/

t_texture   *texture_create(t_game *game, char *texture_path)
{
    t_texture   *texture;

    // Additional sanity checks
    if (!game || !game->mlx) {
        ft_printf("Error: Invalid game or MLX context\n");
        return (NULL);
    }

    if (!texture_path) {
        ft_printf("Error: Texture path is NULL\n");
        return (NULL);
    }

    texture = (t_texture *)ft_calloc(1, sizeof(t_texture));
    if (!texture)
    {
        ft_printf("Error: Failed to allocate memory for texture\n");
        return (NULL);
    }

    // Print the full path for debugging
    ft_printf("Attempting to load texture from path: %s\n", texture_path);

    texture->img = mlx_xpm_file_to_image(game->mlx, texture_path,
            &texture->width, &texture->height);
    if (!texture->img)
    {
        ft_printf("Error: Failed to load texture: %s\n", texture_path);
        free(texture);
        return (NULL);
    }

    // Check dimensions
    if (texture->width <= 0 || texture->height <= 0) {
        ft_printf("Error: Invalid texture dimensions: %dx%d\n", 
                  texture->width, texture->height);
        mlx_destroy_image(game->mlx, texture->img);
        free(texture);
        return (NULL);
    }

    texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
            &texture->line_len, &texture->endian);
    
    if (!texture->addr) {
        ft_printf("Error: Failed to get texture data address\n");
        mlx_destroy_image(game->mlx, texture->img);
        free(texture);
        return (NULL);
    }

    return (texture);
}

// void	resize_texture(t_texture *src, t_texture *dst)
// {
// 	t_vector_i	pos;
// 	t_vector_i	src_pos;
// 	double		scale_x;
// 	double		scale_y;
// 	int			color;

// 	scale_x = get_scale_factor(src->width, dst->width);
// 	scale_y = get_scale_factor(src->height, dst->height);
// 	pos.y = -1;
// 	while (++pos.y < dst->height)
// 	{
// 		pos.x = -1;
// 		while (++pos.x < dst->width)
// 		{
// 			src_pos = get_scale_coords(pos, scale_x, scale_y);
// 			color = get_texture_pixel(src, src_pos.x, src_pos.y);
// 			if ((color & 0xFFC0CB) == 0)
// 				continue ;
// 			draw_texture_pixel(dst, pos.x, pos.y, color);
// 		}
// 	}
// }
