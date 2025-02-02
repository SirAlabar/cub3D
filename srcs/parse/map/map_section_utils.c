/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_section_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 19:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/02 19:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Extracts sector number from sector identifier
 * Format: sN where N is a number (e.g., "s0", "s1")
 * Returns false if number is invalid or out of bounds
 */
bool	get_sector_number(char *str, int *number)
{
	char	*trimmed;

	trimmed = ft_strtrim(str, " \t\n\r");
	if (!trimmed || trimmed[0] != 's')
	{
		free(trimmed);
		return (false);
	}
	*number = ft_atoi(trimmed + 1);
	free(trimmed);
	if (*number < 0 || *number >= MAX_SECTORS)
	{
		ft_printf(RED"Invalid sector number: %d\n"DEFAULT, *number);
		return (false);
	}
	return (true);
}

/*
 * Validates sector height values
 * Ensures floor_height is below ceiling_height
 * Converts integer values to fixed point representation
 */
bool	validate_heights(t_sector *sector, int floor, int ceiling)
{
	sector->floor_height = int_to_fixed32(floor);
	sector->ceiling_height = int_to_fixed32(ceiling);
	if (sector->floor_height >= sector->ceiling_height)
		return (false);
	return (true);
}

/*
 * Validates sector light level
 * Light must be between 0 and 255
 * Returns false if light value is out of range
 */
bool	validate_light(t_sector *sector, int light)
{
	if (light < 0 || light > 255)
		return (false);
	sector->light = light;
	return (true);
}
