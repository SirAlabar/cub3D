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
 * Extracts linedef number from identifier
 * Format: lN where N is number
 * Returns false if format is invalid
 */
bool	get_linedef_number(char *str, int *number)
{
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\n\r");
	if (!trimmed || trimmed[0] != 'l')
	{
		free(trimmed);
		return (false);
	}
	*number = ft_atoi(trimmed + 1);
	free(trimmed);
	if (*number < 0 || *number >= MAX_LINEDEFS)
	{
		ft_printf(RED"Invalid linedef number: %d\n"DEFAULT, *number);
		return (false);
	}
	return (true);
}
