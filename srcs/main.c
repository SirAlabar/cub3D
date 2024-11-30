/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2024/11/29 16:45:34 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cube3d.h>

int	main(int ac, char **av)
{
	(void) ac;
	(void) av;
	printf("Setup\n");
	ft_putendl_fd("setup?\n", 1);
	void *mlx_ptr;
    void *win_ptr;

	mlx_ptr = mlx_init(); // Inicializa a conexão com a MLX
	win_ptr = mlx_new_window(mlx_ptr, 300, 300, "Simple Screen");
    mlx_loop(mlx_ptr);
	return (0);
}
