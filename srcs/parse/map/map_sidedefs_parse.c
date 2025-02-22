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

    // Debug: imprimir linha original
    ft_printf(YELLOW"Parsing sidedef line: [%s]\n"DEFAULT, line);

    tokens = ft_split(line, '=');
    if (!tokens || !tokens[0] || !tokens[1])
    {
        ft_printf(RED"Failed to split line. Tokens are NULL or incomplete.\n"DEFAULT);
        return (free_split(tokens), false);
    }

    // Debug: imprimir tokens
    ft_printf("Token 0: [%s]\n", tokens[0]);
    ft_printf("Token 1: [%s]\n", tokens[1]);

    success = false;
    trimmed = ft_strtrim(tokens[0], " \t\n\r");

    // Debug: imprimir token após trim
    ft_printf("Trimmed token 0: [%s]\n", trimmed);

    if (get_sidedef_number(trimmed, &sidedef_num))
    {
        ft_printf("Sidedef number: %d\n", sidedef_num);
        
        free(trimmed);
        trimmed = ft_strtrim(tokens[1], " \t\n\r");
        
        // Debug: imprimir token 1 após trim
        ft_printf("Trimmed token 1: [%s]\n", trimmed);

        ft_bzero(&new_sidedef, sizeof(t_sidedef));
       
        if (get_sidedef_data(trimmed, &new_sidedef))
        {
            // Debug: imprimir detalhes do sidedef
            ft_printf("Sidedef data parsed successfully\n");
            ft_printf("X offset: %d\n", new_sidedef.x_offset);
            ft_printf("Y offset: %d\n", new_sidedef.y_offset);
            ft_printf("Upper texture: %s\n", new_sidedef.upper_texture ? new_sidedef.upper_texture : "NULL");
            ft_printf("Middle texture: %s\n", new_sidedef.middle_texture ? new_sidedef.middle_texture : "NULL");
            ft_printf("Lower texture: %s\n", new_sidedef.lower_texture ? new_sidedef.lower_texture : "NULL");
            ft_printf("Sector: %d\n", new_sidedef.sector);

            if (validate_sidedef(&new_sidedef))
            {
                map->sidedefs[sidedef_num] = new_sidedef;
                if (sidedef_num >= map->sidedef_count)
                    map->sidedef_count = sidedef_num + 1;
                success = true;
            }
            else
            {
                ft_printf(RED"Sidedef validation failed\n"DEFAULT);
            }
        }
        else
        {
            ft_printf(RED"Failed to parse sidedef data\n"DEFAULT);
        }
    }
    else
    {
        ft_printf(RED"Failed to get sidedef number\n"DEFAULT);
    }

    free(trimmed);
    free_split(tokens);
    return (success);
}
