
# include <cub3d.h>

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

bool validade_map_obj (t_game *game, int *avaliable_spaces)
{
    int spaces;
    t_vector_i pos;

    if (game->map.width < MIN_MAP_SIZE || game->map.height < MIN_MAP_SIZE)
        return (false);
    spaces = 0;
    pos.y = -1;
    while (++y < game->map.height)
    {
        pos.x = -1;
        while (++x < game->map.width)
        {
            if (game->map.grid[y][x] == '0')
                spaces++;     
        }
    }
    if (spaces < MIN_EMPTY_SPACES)
        return (false);
    *avaliable_spaces = spaces;
    return (true);
}

bool validade_tex_obj(t_object_type type)
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

