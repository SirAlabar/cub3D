/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_room.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:54:33 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/15 20:27:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static bool verify_tex_file(const char *path)
{
    int fd;

    if (!path)
        return (false);
    fd = open(path, O_RDONLY);
    if (fd == -1)
        return (false);
    close(fd);
    return (true);
}

static bool	verify_anim_tex(t_object_config config)
{
	int	i;

	i = 0;
	while (i < config.num_frames)
	{
		if (!verify_tex_file(config.frame_paths[i]))
		{
			return (false);
		}
		i++;
	}
	return (true);
}

bool validade_map_obj(t_game *game, int *avaliable_spaces)
{
    int spaces;
    t_vector_i pos;

    if (game->map.width < MIN_MAP_SIZE || game->map.height < MIN_MAP_SIZE)
        return (false);
    spaces = 0;
    pos.y = -1;
    while (++pos.y < game->map.height)
    {
        pos.x = -1;
        while (++pos.x < game->map.width)
        {
            if (game->map.grid[pos.y][pos.x] == '0')
                spaces++;     
        }
    }
    if (spaces < MIN_EMPTY_SPACES)
        return (false);
    *avaliable_spaces = spaces;
    return (true);
}

bool validate_tex_obj(t_object_type type)
{
    t_object_config config;

    init_object_config(&config, type);
    if (config.is_animated)
        return (verify_anim_tex(config));
    return (verify_tex_file(config.texture_path));
}

bool	validate_all_textures(int *valid_types)
{
	int		type;
	int		count;
	bool	result;

	type = 0;
	count = 0;
	result = false;
	while (type < OBJ_TYPES_COUNT)
	{
		if (validate_tex_obj(type))
		{
			valid_types[count] = type;
			count++;
			result = true;
		}
		type++;
	}
	valid_types[count] = -1;
	return (result);
}

