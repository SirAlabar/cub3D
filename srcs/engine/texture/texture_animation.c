
#include <cub3d.h>

void update_weapon_animation(t_game *game)
{
    double current_time = get_time_ms();
    
    if (game->p1.is_firing)
    {
        if (current_time - game->p1.last_fire > 50)
        {
            game->p1.current_frame = (game->p1.current_frame + 1) % 4;
            game->p1.last_fire = current_time;
            
            if (game->p1.current_frame == 0)
                game->p1.is_firing = 0;
        }
    }
    else if (game->p1.keys.w || game->p1.keys.s || game->p1.keys.a || game->p1.keys.d)
    {
        if (current_time - game->p1.last_step > 250)
        {
            game->p1.current_frame = game->p1.current_frame == 0 ? 1 : 0;
            game->p1.last_step = current_time;
        }
    }
    else
    {
        game->p1.current_frame = 0;
    }
}
