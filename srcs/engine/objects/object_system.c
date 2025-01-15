
#include <objects.h>

static bool	try_place_object(t_game *game, t_object_type type)
{
	t_vector		pos;
	t_object_config	config;
	t_placement_type	placement;

	init_object_config(&config, type);
	placement = config.placement;
	pos = get_random_position(game, placement);
	if (pos.x == -1)
		return (false);

	if (game->map.num_sprites >= game->map.max_sprites)
		return (false);

	return (init_sprite(game, &game->map.sprites[game->map.num_sprites++],
			pos, type));
}
