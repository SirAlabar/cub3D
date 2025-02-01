/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_tables_8192.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:09:37 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/30 12:10:57 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
* Creates a lookup table for sine values
* Table has 8192 entries for high precision
* Each entry is angle mapped from 0 to 2π in fixed point
*/
static t_fixed32	*create_sin_table_8192(void)
{
    t_fixed32	*table;
    int			i;
    double		angle;

    table = (t_fixed32 *)malloc(FINEANGLES * sizeof(t_fixed32));
    if (!table)
        return (NULL);
    i = 0;
    while (i < FINEANGLES)
    {
        angle = (i * 2.0 * M_PI) / FINEANGLES;
        table[i] = float_to_fixed32(sin(angle));
        i++;
    }
    return (table);
}

/*
* Creates a lookup table for tangent values
* Handles special cases where cos(angle) = 0
* Returns INT32_MAX/MIN for undefined points
*/
static t_fixed32	*create_tan_table_8192(void)
{
   t_fixed32	*table;
   int			i;
   double		angle;

   table = (t_fixed32 *)malloc(FINEANGLES * sizeof(t_fixed32));
   if (!table)
       return (NULL);
   i = 0;
   while (i < FINEANGLES)
   {
       angle = (i * 2.0 * M_PI) / FINEANGLES;
       if (cos(angle) == 0)
       {
           if (angle > M_PI / 2)
               table[i] = INT32_MIN;
           else
               table[i] = INT32_MAX;
       }
       else
           table[i] = float_to_fixed32(tan(angle));
       i++;
   }
   return (table);
}

/*
* Initializes the fixed point lookup tables
* Creates tables for sine and tangent
* Returns NULL if any allocation fails
*/
t_fixed_tables_8192	*init_fixed_tables_8192(void)
{
	t_fixed_tables_8192	*tables;

	tables = (t_fixed_tables_8192 *)malloc(sizeof(t_fixed_tables_8192));
	if (!tables)
		return (NULL);
	tables->sin_table = create_sin_table_8192();
	tables->tan_table = create_tan_table_8192();
	if (!tables->sin_table || !tables->tan_table)
	{
		destroy_fixed_tables_8192(tables);
		return (NULL);
	}
	tables->is_init = true;
	return (tables);
}

/*
* Frees all allocated memory for lookup tables
* Safely handles NULL pointers
* Used during cleanup or error handling
*/
void	destroy_fixed_tables_8192(t_fixed_tables_8192 *tables)
{
	if (!tables)
		return ;
	if (tables->sin_table)
		free(tables->sin_table);
	if (tables->tan_table)
		free(tables->tan_table);
	free(tables);
}
