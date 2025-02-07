/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:37:53 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/29 20:06:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_POINT_H
# define FIXED_POINT_H

# include <stdbool.h>
# include <stdint.h>

/*
Fixed point configuration
*/
# define FIXED_POINT_BITS 16
# define FIXED_POINT_SCALE 65536 /*(1 << FIXED_POINT_BITS)*/
# define FIXED_POINT_MASK 65535  /*(FIXED_POINT_SCALE - 1)*/

/*
Common constants in fixed point representation
*/
# define FIXED32_HALF 32768  /* (FIXED32_SCALE >> 1) */
# define FIXED32_PI 205887   /* π in fixed point (≈3.14159 * 2^16) */
# define FIXED32_2PI 411775  /* 2π in fixed point */
# define FIXED32_PI_2 102944 /* π/2 in fixed point */
# define FIXED32_E 178145    /* e in fixed point */

# define FINEANGLES 8192
# define FINEMASK 8191
# define FINEANGLEBITS 13
# define ANGLETOFINESHIFT 19

# define ANG90 1073741824  /* 0x40000000 */
# define ANG180 2147483648 /* 0x80000000 */
# define ANG270 3221225472 /* 0xc0000000 */
# define ANG360 4294967295 /* 0xffffffff */

/*
Types
*/
typedef int32_t		t_fixed32;
typedef int64_t		t_fixed64;

typedef struct s_fixed_vec32
{
	t_fixed32		x;
	t_fixed32		y;
}					t_fixed_vec32;

typedef struct s_fixed_vec64
{
	t_fixed64		x;
	t_fixed64		y;
}					t_fixed_vec64;

typedef struct s_fixed_tables_8192
{
	t_fixed32		*sin_table;
	t_fixed32		*tan_table;
	bool			is_init;
}					t_fixed_tables_8192;

/*
** Basic conversion functions (fixed_conversions.c)
*/
t_fixed32			float_to_fixed32(float value);
t_fixed32			int_to_fixed32(int value);
float				fixed32_to_float(t_fixed32 value);
int					fixed32_to_int(t_fixed32 value);

/*
** Basic arithmetic operations (fixed32_vec_basic_operations.c)
*/
t_fixed32			fixed32_add(t_fixed32 a, t_fixed32 b);
t_fixed32			fixed32_sub(t_fixed32 a, t_fixed32 b);
t_fixed32			fixed32_mul(t_fixed32 a, t_fixed32 b);
t_fixed32			fixed32_div(t_fixed32 a, t_fixed32 b);
t_fixed32			fixed32_abs(t_fixed32 value);
t_fixed32			fix_max(t_fixed32 a, t_fixed32 b);
t_fixed32			fix_min(t_fixed32 a, t_fixed32 b);

/*
** Advanced operations (fixed32_vec_adv_operations.c)
*/
t_fixed32			fixed32_sqrt(t_fixed32 value);
t_fixed32			fixed32_sin(t_fixed32 angle);
t_fixed32			fixed32_cos(t_fixed32 angle);
t_fixed32			fixed32_tan(t_fixed32 angle);
t_fixed32			fixed32_atan2(t_fixed32 y, t_fixed32 x);

/*
** Vector operations (fixed32_vec_operations.c)
*/
t_fixed32			fixed32_vec_dot(t_fixed_vec32 v1, t_fixed_vec32 v2);
t_fixed32			fixed32_vec_mag_sq(t_fixed_vec32 v);
t_fixed32			fixed32_vec_mag(t_fixed_vec32 v);
t_fixed_vec32		fixed32_vec_normalize(t_fixed_vec32 v);

/*
** Angular vector operations (fixed32_vec_angular_operations.c)
*/
t_fixed32			fixed32_vec_heading(t_fixed_vec32 v);
t_fixed_vec32		fixed32_vec_rotate(t_fixed_vec32 v, t_fixed32 angle);
t_fixed_vec32		fixed32_vec_lerp(t_fixed_vec32 start, t_fixed_vec32 end,
						t_fixed32 amt);
t_fixed32			fixed32_vec_dist(t_fixed_vec32 v1, t_fixed_vec32 v2);
t_fixed_vec32		fixed32_vec_limit(t_fixed_vec32 v, t_fixed32 max);

/*
** Lookup table operations (fixed_tables.c)
*/
t_fixed_tables_8192	*init_fixed_tables_8192(void);
void				destroy_fixed_tables_8192(t_fixed_tables_8192 *tables);
t_fixed32			get_sin_8192(t_fixed_tables_8192 *tables, t_fixed32 angle);
t_fixed32			get_cos_8192(t_fixed_tables_8192 *tables, t_fixed32 angle);
t_fixed32			get_tan_8192(t_fixed_tables_8192 *tables, t_fixed32 angle);

#endif