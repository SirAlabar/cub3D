/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils_linux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 18:27:36 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/01 18:30:19 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	cleanup_mlx(void *mlx)
{
	mlx_destroy_display(mlx);
	free(mlx);
}

void	destroy_window(void *mlx, void *win)
{
	mlx_clear_window(mlx, win);
	mlx_destroy_window(mlx, win);
}

void	*init_window(void *mlx, int width, int height, char *title)
{
	void	*win;

	win = mlx_new_window(mlx, width, height, title);
	return (win);
}
