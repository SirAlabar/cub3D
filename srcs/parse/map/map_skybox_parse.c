

#include <cub3d.h>

bool    parse_skybox_section(char *line, t_doom_map *map)
{
    char    *cleaned;
    
    cleaned = clean_line(line);
    if (!cleaned)
        return (false);
    if (map->skybox_path)
    {
        free(map->skybox_path);
        map->skybox_path = NULL;
    }
    map->skybox_path = ft_strdup(cleaned);
    free(cleaned);
    if (!texture_exists(map->skybox_path))
    {
        ft_printf(RED"Invalid skybox texture path: %s\n"DEFAULT, 
                 map->skybox_path);
        free(map->skybox_path);
        map->skybox_path = NULL;
        return (false);
    }
    return (true);
}
