
#include <cub3d.h>

void    init_game(t_game *game)
{
    ft_memset(game, 0, sizeof(t_game));
    game->map.floor_color = 0x808080;
    game->map.ceiling_color = 0x87CEEB;
    game->player.move_speed = 0.1;
    game->player.rot_speed = 0.05;
}
