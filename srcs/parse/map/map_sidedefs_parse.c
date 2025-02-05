/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_sidedefs_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:30:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/03 16:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

/*
 * Processes sidedef data into component values
 * Format: x_offset,y_offset,upper_tex,middle_tex,lower_tex,sector
 * Returns false if any component is invalid
 */
bool get_sidedef_data(char *data, t_sidedef *sidedef)
{
    char    **parts;
    int     sector_num;
    bool    success;

    parts = ft_split(data, ',');
    if (!parts || !parts[0] || !parts[1] || !parts[2] || !parts[3] 
        || !parts[4] || !parts[5] || parts[6])
        return (free_split(parts), false);
    success = false;
    sidedef->x_offset = int_to_fixed32(ft_atoi(parts[0]));
    sidedef->y_offset = int_to_fixed32(ft_atoi(parts[1]));
    if (parse_texture_path(parts[2], &sidedef->upper_texture) &&
        parse_texture_path(parts[3], &sidedef->middle_texture) &&
        parse_texture_path(parts[4], &sidedef->lower_texture))
    {
        if (get_sector_number(parts[5], &sector_num))
        {
            sidedef->sector = sector_num;
            success = true;
        }
    }
    free_split(parts);
    return (success);
}

/*
 * Parses a sidedef definition line
 * Format: sdN = x_offset,y_offset,upper_tex,middle_tex,lower_tex,sector
 * Returns false if parsing fails
 */
bool    parse_sidedefs_section(char *line, t_doom_map *map)
{
    char        **tokens;
    int         sidedef_num;
    char        *trimmed;
    t_sidedef   new_sidedef;
    bool        success;

    tokens = ft_split(line, '=');
    if (!tokens || !tokens[0] || !tokens[1])
        return (free_split(tokens), false);

    success = false;
    trimmed = ft_strtrim(tokens[0], " \t\n\r");
    if (get_sidedef_number(trimmed, &sidedef_num))
    {
        free(trimmed);
        trimmed = ft_strtrim(tokens[1], " \t\n\r");
        ft_bzero(&new_sidedef, sizeof(t_sidedef));
        
        if (get_sidedef_data(trimmed, &new_sidedef))
        {
            if (validate_sidedef(&new_sidedef, map))
            {
                map->sidedefs[sidedef_num] = new_sidedef;
                if (sidedef_num >= map->sidedef_count)
                    map->sidedef_count = sidedef_num + 1;
                success = true;
            }
        }
    }
    free(trimmed);
    free_split(tokens);
    return (success);
}
