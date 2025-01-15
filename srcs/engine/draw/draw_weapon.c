
#include <cub3d.h>

void draw_weapon(t_game *game)
{
    t_texture *current_gun = &game->p1.gun_anim[game->p1.current_frame];
    
    int gun_width = WINDOW_WIDTH / 2;
    int gun_height = WINDOW_HEIGHT / 2;
    int gun_x = (WINDOW_WIDTH - gun_width) / 2;
    int gun_y = WINDOW_HEIGHT - gun_height;
    
    t_texture resized;
    resized.width = gun_width;
    resized.height = gun_height;
    resized.img = mlx_new_image(game->mlx, gun_width, gun_height);
    resized.addr = mlx_get_data_addr(resized.img, &resized.bpp, 
                                    &resized.line_len, &resized.endian);
    
    resize_texture(game, current_gun, &resized);
    mlx_put_image_to_window(game->mlx, game->win, resized.img, gun_x, gun_y);
    mlx_destroy_image(game->mlx, resized.img);
}