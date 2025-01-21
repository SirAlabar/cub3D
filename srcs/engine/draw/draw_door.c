/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:20:50 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/09 20:13:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void    update_doors(t_game *game)
{
    t_door_system    *ds = game->door_system;
    const double     delta_time = 1.0 / 60.0;
    int              i;

    i = 0;
    while (i < ds->door_count)
    {
        t_door    *door = &ds->doors[i];
        if (door->state == DOOR_OPENING)
        {
            door->animation += DOOR_SPEED * delta_time;
            if (door->animation >= 1.0)
            {
                door->animation = 1.0;
                door->state = DOOR_OPEN;
                door->timer = DOOR_STAY_OPEN_TIME;
            }
        }
        else if (door->state == DOOR_OPEN)
        {
            door->timer -= delta_time;
            if (door->timer <= 0)
                door->state = DOOR_CLOSING;
        }
        else if (door->state == DOOR_CLOSING)
        {
            door->animation -= DOOR_SPEED * delta_time;
            if (door->animation <= 0.0)
            {
                door->animation = 0.0;
                door->state = DOOR_CLOSED;
            }
        }
        i++;
    }
}

void door_sliding(t_ray *ray, t_game *game, t_door *door)
{
    if (door->state == DOOR_OPEN)
        return;

    // Calcula a posição relativa do raio na porta
    double door_hit_pos;
    if (ray->side == 0)
        door_hit_pos = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
    else
        door_hit_pos = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
    door_hit_pos -= floor(door_hit_pos);

    if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
    {
        double animation_threshold = (door->state == DOOR_OPENING) ?
                                   (1.0 - door->animation) : door->animation;

        // Adiciona uma pequena margem para evitar artefatos
        double margin = 0.001;
        if (door_hit_pos <= animation_threshold - margin)
        {
            ray->hit = true;
            ray->is_door = true;
        }
        else
        {
            if (ray->side == 0)
            {
                ray->side_dist.x += ray->delta_dist.x;
                ray->map_x += ray->step_x;
            }
            else
            {
                ray->side_dist.y += ray->delta_dist.y;
                ray->map_y += ray->step_y;
            }
            ray->hit = false;
            ray->is_door = false;
        }
    }
    else
    {
        ray->hit = true;
        ray->is_door = true;
    }
}

/*
static int	get_line_height(double perp_wall_dist)
{
	return ((int)(WINDOW_HEIGHT / perp_wall_dist));
}

static void	get_draw_limits(int line_height, int *start, int *end)
{
	*start = -line_height / 2 + WINDOW_HEIGHT / 2;
	if (*start < 0)
		*start = 0;
	*end = line_height / 2 + WINDOW_HEIGHT / 2;
	if (*end >= WINDOW_HEIGHT)
		*end = WINDOW_HEIGHT - 1;
}

static double	get_wall_x(t_game *game, t_ray *ray)
{
	t_vector	wall_pos;
	t_vector	ray_vector;

	ray_vector = vector_mult(ray->dir, ray->perp_wall_dist);
	wall_pos = vector_add(game->p1.pos, ray_vector);
	if (ray->side == 0)
		return (wall_pos.y - floor(wall_pos.y));
	return (wall_pos.x - floor(wall_pos.x));
}

static void	draw_vertical_line(t_game *game, t_door *door, int x, t_ray *ray)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	t_vector_i	tex;
	double		step;
	double		tex_pos;

	line_height = get_line_height(ray->perp_wall_dist);
	get_draw_limits(line_height, &draw_start, &draw_end);
	tex.x = (int)(get_wall_x(game, ray) * game->door_system->door_texture.width);
	tex.x += (int)(door->animation * game->door_system->door_texture.width);
	step = 1.0 * game->door_system->door_texture.height / line_height;
	tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	while (draw_start < draw_end)
	{
		tex.y = (int)tex_pos & (game->door_system->door_texture.height - 1);
		tex_pos += step;
		draw_pixel(game, x, draw_start, apply_shade(get_texture_pixel(
			&game->door_system->door_texture, tex.x, tex.y),
			1.0 / (1.0 + ray->perp_wall_dist * 0.1)));
		draw_start++;
	}
}*/
/*
double get_door_distance(t_game *game, t_door *door)
{
    t_vector door_pos = vector_create(door->position.x + 0.5, door->position.y + 0.5);
    t_vector player_pos = game->p1.pos;
    return vector_dist(door_pos, player_pos);
}

void render_door(t_game *game, t_ray *ray, int x)
{
    t_door *door = find_door(game, ray->map_x, ray->map_y);
    if (!door)
        return;

    // Calcula a posição na textura da porta
    double wall_x;
    if (ray->side == 0)
        wall_x = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
    else
        wall_x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
    wall_x -= floor(wall_x);

    // Ajusta a posição da textura baseado na orientação da porta
    int tex_x = (int)(wall_x * game->door_system->door_texture.width);
    if ((ray->side == 0 && ray->dir.x < 0) || 
        (ray->side == 1 && ray->dir.y > 0))
        tex_x = game->door_system->door_texture.width - tex_x - 1;
        
    // Ajusta a posição da textura baseado na animação da porta
    tex_x += (int)(door->animation * game->door_system->door_texture.width);
    
    // Calcula a altura da linha da porta
    double step = 1.0 * game->door_system->door_texture.height / ray->line_height;
    double tex_pos = (ray->draw_start - WINDOW_HEIGHT / 2 + ray->line_height / 2) * step;

    // Renderiza a linha vertical da porta
    for (int y = ray->draw_start; y < ray->draw_end; y++)
    {
        int tex_y = (int)tex_pos & (game->door_system->door_texture.height - 1);
        double shade = 1.0 / (1.0 + ray->perp_wall_dist * 0.04);
        unsigned int color = get_texture_pixel(&game->door_system->door_texture, 
                                            tex_x, tex_y);
        
        // Aplica sombreamento baseado na distância
        color = apply_shade(color, shade);
        
        // Se a porta estiver em movimento, aplica um efeito de transparência
        if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
        {
            double alpha = 1.0 - door->animation;
            if (alpha < 1.0)
                color = apply_shade(color, alpha);
        }
        
        draw_pixel(game, x, y, color);
        tex_pos += step;
    }
}

*/