
#include <cub3d.h>

void    draw_weapon(t_game *game)
{
    t_texture   *weapon;
    int         pos_x;
    int         pos_y;
    int         x;
    int         y;
    unsigned int color;

    weapon = &game->p1.frames[game->p1.current_frame];
    pos_x = (WINDOW_WIDTH - weapon->width) / 2;
    pos_y = WINDOW_HEIGHT - weapon->height + 100;
    y = 0;
    while (y < weapon->height)
    {
        x = 0;
        while (x < weapon->width)
        {
            if (pos_x + x >= 0 && pos_x + x < WINDOW_WIDTH && 
                pos_y + y >= 0 && pos_y + y < WINDOW_HEIGHT)
            {
                color = get_texture_pixel(weapon, x, y);
                if (color != 0xFF000000)
                    draw_pixel(game, pos_x + x, pos_y + y, color);
            }
            x++;
        }
        y++;
    }
}

void    update_weapon_frame(t_game *game)
{
    static double    last_time;
    double          current_time;
    double          frame_duration;

    if (!game->p1.is_shooting)
    {
        game->p1.current_frame = 0;
        return ;
    }
    current_time = get_time_ms() / 1000.0;
    frame_duration = 0.05;
    if (last_time == 0)
        last_time = current_time;
    if (current_time - last_time >= frame_duration)
    {
        game->p1.current_frame++;
        last_time = current_time;
        if (game->p1.current_frame >= P1_FRAMES)
        {
            game->p1.current_frame = 0;
            game->p1.is_shooting = false;
            last_time = 0;
        }
    }
}