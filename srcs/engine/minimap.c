#include <cub3d.h>

#define MINIMAP_CELL_SIZE 12
#define MINIMAP_PADDING 20
#define PLAYER_DOT_SIZE 6
#define MINIMAP_VIEW_SIZE 11  // Must be odd number to center player

static void draw_minimap_cell(t_game *game, int screen_x, int screen_y, int color)
{
    int start_x = MINIMAP_PADDING + (screen_x * MINIMAP_CELL_SIZE);
    int start_y = WINDOW_HEIGHT - MINIMAP_PADDING - ((MINIMAP_VIEW_SIZE - screen_y) * MINIMAP_CELL_SIZE);
    int i = -1;
    int j;

    while (++i < MINIMAP_CELL_SIZE)
    {
        j = -1;
        while (++j < MINIMAP_CELL_SIZE)
        {
            draw_pixel(game, start_x + j, start_y + i, color);
        }
    }
}

static void draw_player_dot(t_game *game)
{
    // Player is always at the center of the minimap viewport
    int center_x = MINIMAP_PADDING + ((MINIMAP_VIEW_SIZE / 2) * MINIMAP_CELL_SIZE) + (MINIMAP_CELL_SIZE / 2);
    int center_y = WINDOW_HEIGHT - MINIMAP_PADDING - ((MINIMAP_VIEW_SIZE / 2) * MINIMAP_CELL_SIZE) - (MINIMAP_CELL_SIZE / 2);
    int half_size = PLAYER_DOT_SIZE / 2;
    int i, j;

    // Draw player position as a red dot
    for (i = -half_size; i < half_size; i++)
    {
        for (j = -half_size; j < half_size; j++)
        {
            if ((i * i + j * j) <= (half_size * half_size))
                draw_pixel(game, center_x + j, center_y + i, 0xFF0000);
        }
    }

    // Draw direction line
    int line_length = MINIMAP_CELL_SIZE;
    int end_x = center_x + game->p1.dir.y * line_length;
    int end_y = center_y + game->p1.dir.x * line_length;
    
    // Draw a simple line for direction
    int dx = end_x - center_x;
    int dy = end_y - center_y;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    float x_inc = dx / (float)steps;
    float y_inc = dy / (float)steps;
    float x = center_x;
    float y = center_y;
    
    for (i = 0; i < steps; i++)
    {
        draw_pixel(game, round(x), round(y), 0xFF0000);
        x += x_inc;
        y += y_inc;
    }
}

static unsigned int darken_color(unsigned int original, float factor)
{
    unsigned char r = ((original >> 16) & 0xFF) * factor;
    unsigned char g = ((original >> 8) & 0xFF) * factor;
    unsigned char b = (original & 0xFF) * factor;
    
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

static unsigned int get_pixel_color(t_game *game, int x, int y)
{
    char *pixel;
    
    if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
        return 0;
        
    pixel = game->addr + (y * game->line_length + x * (game->bits_per_pixel / 8));
    return *(unsigned int*)pixel;
}

static void draw_minimap_background(t_game *game)
{
    int width = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING * 2);
    int height = MINIMAP_VIEW_SIZE * MINIMAP_CELL_SIZE + (MINIMAP_PADDING * 2);
    int i, j;
    unsigned int original_color;
    unsigned int darkened_color;

    // Draw semi-transparent background by darkening existing pixels
    int base_y = WINDOW_HEIGHT - height;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i < MINIMAP_PADDING || i >= height - MINIMAP_PADDING ||
                j < MINIMAP_PADDING || j >= width - MINIMAP_PADDING)
                continue;
            // Adjust y position to be from bottom
            int draw_y = base_y + i;
                
            original_color = get_pixel_color(game, j, draw_y);
            darkened_color = darken_color(original_color, 0.5); // Darken by 50%
            draw_pixel(game, j, draw_y, darkened_color);
        }
    }
}

static bool is_valid_map_position(t_game *game, int x, int y)
{
    if (x < 0 || y < 0 || x >= game->map.height || y >= game->map.width)
        return false;
    if (y >= (int)ft_strlen(game->map.grid[x]))
        return false;
    return true;
}

void draw_minimap(t_game *game)
{
    int map_x, map_y;
    int screen_x, screen_y;
    int player_map_x = (int)game->p1.pos.x;
    int player_map_y = (int)game->p1.pos.y;
    int view_radius = MINIMAP_VIEW_SIZE / 2;

    draw_minimap_background(game);

    // Draw visible portion of the map
    for (screen_y = 0; screen_y < MINIMAP_VIEW_SIZE; screen_y++)
    {
        for (screen_x = 0; screen_x < MINIMAP_VIEW_SIZE; screen_x++)
        {
            map_x = player_map_x + (screen_y - view_radius);
            map_y = player_map_y + (screen_x - view_radius);

            if (!is_valid_map_position(game, map_x, map_y))
                continue;

            if (game->map.grid[map_x][map_y] == '1')
                draw_minimap_cell(game, screen_x, screen_y, 0xFFFFFF);  // Walls in white
            else if (game->map.grid[map_x][map_y] == '0' || 
                    ft_strchr("NSEW", game->map.grid[map_x][map_y]))
                draw_minimap_cell(game, screen_x, screen_y, 0x808080);  // Floor in gray
        }
    }

    // Draw player in the center
    draw_player_dot(game);
}
