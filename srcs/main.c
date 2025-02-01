/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:37:53 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/11 13:48:06 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
/*
int	close_window(t_game *game)
{
	mlx_loop_end(game->mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	ft_bzero(game, sizeof(t_game));
	if (!game)
		return (1);
	if (!validate_map_extension(argc, argv))
		return (free(game), 1);
	if (!checker(game, argv[1]))
		return (1);
	game->mlx = mlx_init();
	if (!game->mlx)
		return (cleanup_game(game), 1);
	init_game(game);
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->win)
		return (cleanup_game(game), 1);
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_hook(game->win, 6, 1L << 6, mouse_wrapper, game);
	mlx_loop_hook(game->mlx, engine_render_frame, game);
	mlx_loop(game->mlx);
	cleanup_game(game);
	return (0);
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_fixed_point.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:06:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/30 15:08:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fixed_point.h"
#include <stdio.h>
#include <math.h>

void	test_basic_conversions(void)
{
	float		test_float;
	t_fixed32	fixed;
	float		back_to_float;
	int			test_int;
	t_fixed32	fixed_from_int;
	int			back_to_int;

	printf("\n=== Testing Basic Conversions ===\n");
	test_float = 3.14159;
	fixed = float_to_fixed32(test_float);
	back_to_float = fixed32_to_float(fixed);
	printf("Float to Fixed to Float: %.5f -> %d -> %.5f\n",
		test_float, fixed, back_to_float);

	test_int = 42;
	fixed_from_int = int_to_fixed32(test_int);
	back_to_int = fixed32_to_int(fixed_from_int);
	printf("Int to Fixed to Int: %d -> %d -> %d\n",
		test_int, fixed_from_int, back_to_int);
}

void	test_basic_arithmetic(void)
{
	t_fixed32	a;
	t_fixed32	b;
	float		fa;
	float		fb;

	printf("\n=== Testing Basic Arithmetic ===\n");
	fa = 3.14;
	fb = 2.0;
	a = float_to_fixed32(fa);
	b = float_to_fixed32(fb);

	printf("a = %.2f, b = %.2f\n", fa, fb);
	printf("Add: %.2f + %.2f = %.2f\n",
		fa, fb, fixed32_to_float(fixed32_add(a, b)));
	printf("Sub: %.2f - %.2f = %.2f\n",
		fa, fb, fixed32_to_float(fixed32_sub(a, b)));
	printf("Mul: %.2f * %.2f = %.2f\n",
		fa, fb, fixed32_to_float(fixed32_mul(a, b)));
	printf("Div: %.2f / %.2f = %.2f\n",
		fa, fb, fixed32_to_float(fixed32_div(a, b)));
}

static t_fixed32	rad_to_bam(float rad)
{
    // Normalizar ângulo para [0, 2π]
    while (rad < 0)
        rad += 2.0 * M_PI;
    while (rad >= 2.0 * M_PI)
        rad -= 2.0 * M_PI;
    
    // Converter para BAM
    return ((t_fixed32)(rad * (ANG360 / (2.0 * M_PI))));
}

void	test_trig_functions(t_fixed_tables_8192 *tables)
{
	float		angle;
	t_fixed32	bam_angle;
	float		angles[4];
	int			i;

	printf("\n=== Testing Trigonometric Functions ===\n");
	angles[0] = 0;
	angles[1] = M_PI / 6;  // 30 degrees
	angles[2] = M_PI / 4;  // 45 degrees
	angles[3] = M_PI / 2;  // 90 degrees

	i = 0;
	while (i < 4)
	{
		angle = angles[i];
		bam_angle = rad_to_bam(angle);
		printf("\nAngle: %.2f radians\n", angle);
		printf("Regular Sin: %.4f vs Fixed Sin: %.4f vs Table Sin: %.4f\n",
			sin(angle),
			fixed32_to_float(fixed32_sin(bam_angle)),
			fixed32_to_float(get_sin_8192(tables, bam_angle)));
		printf("Regular Cos: %.4f vs Fixed Cos: %.4f vs Table Cos: %.4f\n",
			cos(angle),
			fixed32_to_float(fixed32_cos(bam_angle)),
			fixed32_to_float(get_cos_8192(tables, bam_angle)));
		i++;
	}
}

void	test_vector_operations(void)
{
	t_fixed_vec32	v1;
	t_fixed_vec32	v2;
	t_fixed32		dot;
	t_fixed32		mag;

	printf("\n=== Testing Vector Operations ===\n");
	v1.x = float_to_fixed32(3.0);
	v1.y = float_to_fixed32(4.0);
	v2.x = float_to_fixed32(1.0);
	v2.y = float_to_fixed32(2.0);

	printf("Vector 1: (%.2f, %.2f)\n",
		fixed32_to_float(v1.x), fixed32_to_float(v1.y));
	printf("Vector 2: (%.2f, %.2f)\n",
		fixed32_to_float(v2.x), fixed32_to_float(v2.y));

	dot = fixed32_vec_dot(v1, v2);
	mag = fixed32_vec_mag(v1);

	printf("Dot Product: %.2f\n", fixed32_to_float(dot));
	printf("Magnitude of v1: %.2f\n", fixed32_to_float(mag));
}

int	main(void)
{
	t_fixed_tables_8192	*tables;

	tables = init_fixed_tables_8192();
	if (!tables)
	{
		printf("Failed to initialize lookup tables\n");
		return (1);
	}

	test_basic_conversions();
	test_basic_arithmetic();
	test_trig_functions(tables);
	test_vector_operations();

	destroy_fixed_tables_8192(tables);
	return (0);
}