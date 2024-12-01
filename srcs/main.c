/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 16:50:09 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	main(int argc, char **argv)
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_game	*game;

	game = NULL;
	ft_bzero(&game, sizeof(t_game *));
	if (argc != 2)
		return (printf("Provide a map\n"), 1);// <- add custom message later
	else
		validate_map(game, argv);
	printf("Setup\n");
	mlx_ptr = mlx_init(); // Inicializa a conexão com a MLX
	win_ptr = mlx_new_window(mlx_ptr, 300, 300, "Simple Screen");
	mlx_loop(mlx_ptr);
	return (0);
}
