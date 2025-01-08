/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 21:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/08 20:14:03 by hluiz-ma         ###   ########.fr       */
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
