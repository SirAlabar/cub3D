/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_section_parse .c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 18:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Extracts the linedef number from an identifier string
 * Format: lN where N is a number (e.g., "l0", "l1")
 * Returns false if string is invalid or number is out of bounds
 */
static bool	get_linedef_number(char *str, int *number)
{
	if (!str || str[0] != 'l')
		return (false);
	*number = ft_atoi(str + 1);
	if (*number < 0 || *number >= MAX_LINEDEFS)
		return (false);
	return (true);
}
