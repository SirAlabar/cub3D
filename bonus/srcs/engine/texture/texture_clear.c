/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:48:34 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/24 19:48:34 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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
