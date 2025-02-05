/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_sidedefs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:45:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/03 16:45:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Extracts sidedef number from identifier
 * Format: sdN where N is number
 * Returns false if format is invalid
 */
bool    get_sidedef_number(char *str, int *number)
{
    char    *trimmed;

    trimmed = ft_strtrim(str, " \t\n\r");
    if (!trimmed || ft_strncmp(trimmed, "sd", 2) != 0)
    {
        free(trimmed);
        return (false);
    }
    *number = ft_atoi(trimmed + 2);
    free(trimmed);
    if (*number < 0 || *number >= MAX_SIDEDEFS)
    {
        ft_printf(RED"Invalid sidedef number: %d\n"DEFAULT, *number);
        return (false);
    }
    return (true);
}

/*
 * Processes texture path string
 * Handles "none" or "-" as valid empty texture indicators
 * Returns false if string is invalid
 */
bool    parse_texture_path(char *str, char **texture_path)
{
    char    *trimmed;

    trimmed = ft_strtrim(str, " \t\n\r");
    if (!trimmed)
        return (false);

    if (ft_strcmp(trimmed, "none") == 0 || ft_strcmp(trimmed, "-") == 0)
    {
        *texture_path = NULL;
        free(trimmed);
        return (true);
    }

    *texture_path = trimmed;
    return (true);
}

/*
 * Validates if at least one texture is specified
 * Checks if texture files exist when specified
 * Returns false if validation fails
 */
bool    validate_sidedef_textures(t_sidedef *sidedef)
{
    if (!sidedef->upper_texture && !sidedef->middle_texture 
        && !sidedef->lower_texture)
    {
        ft_putendl_fd("Error\nSidedef must have at least one texture", 2);
        return (false);
    }

    if (sidedef->upper_texture && !texture_exists(sidedef->upper_texture))
        return (false);
    if (sidedef->middle_texture && !texture_exists(sidedef->middle_texture))
        return (false);
    if (sidedef->lower_texture && !texture_exists(sidedef->lower_texture))
        return (false);

    return (true);
}
