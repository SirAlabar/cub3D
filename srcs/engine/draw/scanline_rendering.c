/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/20 21:43:21 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_scanline_buffer(t_scanline *buffer)
{
	int	x;

	x = -1;
	while (++x < WINDOW_WIDTH)
	{
		buffer->y_top[x] = 0;
		buffer->y_bottom[x] = WINDOW_HEIGHT - 1;
	}
}

void	init_wall_drawing(t_wall *wall)
{
	wall->height = (int)(WINDOW_HEIGHT / wall->ray->perp_wall_dist);
	wall->start = -wall->height / 2 + WINDOW_HEIGHT / 2;
	wall->end = wall->height / 2 + WINDOW_HEIGHT / 2;
	if (wall->start < wall->buffer->y_top[wall->x])
		wall->start = wall->buffer->y_top[wall->x];
	if (wall->end > wall->buffer->y_bottom[wall->x])
		wall->end = wall->buffer->y_bottom[wall->x];
	wall->texture = get_wall_texture(wall->ray, wall->game);
	set_wall_tex_coords(wall);
	wall->step = 1.0 * wall->texture->height / wall->height;
	wall->tex_pos = (wall->start - WINDOW_HEIGHT / 2 + wall->height / 2)
		* wall->step;
}

void	set_wall_tex_coords(t_wall *wall)
{
	if (wall->ray->side == 0)
		wall->pos.x = wall->game->p1.pos.y + wall->ray->perp_wall_dist
			* wall->ray->dir.y;
	else
		wall->pos.x = wall->game->p1.pos.x + wall->ray->perp_wall_dist
			* wall->ray->dir.x;
	wall->pos.x -= floor(wall->pos.x);
	wall->tex.x = (int)(wall->pos.x * wall->texture->width);
	if (wall->ray->side == 0 && wall->ray->dir.x < 0)
		wall->tex.x = wall->texture->width - wall->tex.x - 1;
	if (wall->ray->side == 1 && wall->ray->dir.y > 0)
		wall->tex.x = wall->texture->width - wall->tex.x - 1;
}

static void	put_wall_pixel(t_wall *wall, t_vector_i pos)
{
	int	pixel_pos;

	wall->tex.y = (int)wall->tex_pos & (wall->texture->height - 1);
	wall->color = get_texture_pixel(wall->texture, wall->tex.x, wall->tex.y);
	wall->color = apply_shade(wall->color, 1.0 / (1.0
				+ wall->ray->perp_wall_dist * 0.05));
	pixel_pos = (pos.y * wall->game->line_length) + (pos.x
			* (wall->game->bits_per_pixel / 8));
	*(unsigned int *)(wall->game->addr + pixel_pos) = wall->color;
	wall->tex_pos += wall->step;
}
/*
void	draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
{
	t_wall		wall;
	t_vector_i	pos;
	t_door	*door;

	wall.game = game;
	wall.ray = ray;
	wall.x = x;
	wall.buffer = buffer;
	init_wall_drawing(&wall);

  if (ray->is_door)
    {
        door = find_door(game, ray->map_x, ray->map_y);
        if (door)
        {
            wall.texture = &game->door_system->door_texture;
            
            // Calcula a posição da textura
            double wallx;
            if (ray->side == 0)
                wallx = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
            else
                wallx = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
            wallx -= floor(wallx);

            // Calcula o offset da animação
            int texture_offset = 0;
            if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
            {
                double anim_progress = (door->state == DOOR_OPENING) ? 
                                     door->animation : (1.0 - door->animation);
                texture_offset = (int)(wall.texture->width * anim_progress);
            }

            // Define a coordenada X da textura com o offset da animação
            wall.tex.x = ((int)(wallx * wall.texture->width) + texture_offset) % wall.texture->width;
    if (ray->is_door)
    {
        door = find_door(game, ray->map_x, ray->map_y);
        if (door)
        {
            wall.texture = &game->door_system->door_texture;
            
            // Não renderiza se estiver totalmente aberta
            if (door->state == DOOR_OPEN || 
               (door->state == DOOR_OPENING && door->animation >= 0.8) ||
               (door->state == DOOR_CLOSING && door->animation >= 0.8))
                return;

            // Para animação, corta a porta horizontalmente
            double screen_ratio = (double)x / WINDOW_HEIGHT;
            if (door->state == DOOR_OPENING)
            {
                if (screen_ratio > (1.0 - door->animation))
                    return;
            }
            else if (door->state == DOOR_CLOSING)
            {
                if (screen_ratio > door->animation)
                    return;
            }

            wall.tex.x = (int)(wall.pos.x * wall.texture->width) % wall.texture->width;
        }
    }
            // Só renderiza se o pixel estiver na parte visível da porta
            if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
            {
                if ((door->state == DOOR_OPENING && wall.tex.x >= texture_offset) ||
                    (door->state == DOOR_CLOSING && wall.tex.x < texture_offset))
                {
                    pos.x = x;
                    pos.y = wall.start - 1;
                    while (++pos.y <= wall.end)
                        put_wall_pixel(&wall, pos);
                    buffer->y_top[x] = wall.end + 1;
                    buffer->y_bottom[x] = wall.start - 1;
                }
            }
            else
            {
                pos.x = x;
                pos.y = wall.start - 1;
                while (++pos.y <= wall.end)
                    put_wall_pixel(&wall, pos);
                buffer->y_top[x] = wall.end + 1;
                buffer->y_bottom[x] = wall.start - 1;
            }
            return;
        }
    }


	if (ray->is_door)
	{
		door = find_door(game, ray->map_x, ray->map_y);
		if (door)
		{
			wall.texture = &game->door_system->door_texture;
			// Calcula a posição da textura baseada na animação
			int offset = (int)(door->animation * wall.texture->width);
			wall.tex.x = ((int)(wall.pos.x * wall.texture->width) + offset) 
                        % wall.texture->width;

			wall.texture = &game->door_system->door_texture;
			
			// Calcula a posição da textura com base na animação progressiva
			double animation_progress = door->animation; // Entre 0.0 (fechada) e 1.0 (aberta)
			int offset = (int)(animation_progress * wall.texture->width);

			// Aplica o deslocamento para a textura
			wall.tex.x = ((int)(wall.pos.x * wall.texture->width) + offset) % wall.texture->width;
		}
	}

	pos.x = x;
	pos.y = wall.start - 1;
	while (++pos.y <= wall.end)
		put_wall_pixel(&wall, pos);
	buffer->y_top[x] = wall.end + 1;
	buffer->y_bottom[x] = wall.start - 1;
}*/
void draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
{
    t_wall      wall;
    t_vector_i  pos;
    t_door      *door;

    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
    init_wall_drawing(&wall);
/*
if (ray->is_door)
{
    door = find_door(game, ray->map_x, ray->map_y);
    if (door && door->state != DOOR_OPEN)
    {
        wall.texture = &game->door_system->door_texture;
        
        double wallx;
        if (ray->side == 0)
            wallx = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
        else
            wallx = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
        wallx -= floor(wallx);
        
        wall.tex.x = (int)(wallx * wall.texture->width);
        
        if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
        {
            // Invertemos a lógica do animation_progress aqui
            double animation_progress = (door->state == DOOR_OPENING) ?
                                     (1.0 - door->animation) : door->animation;
            
            // Voltamos com o offset negativo
            int offset = -(int)(wall.texture->width * animation_progress);
            wall.tex.x = (int)((wall.tex.x + offset + wall.texture->width)) % wall.texture->width;
        }
    }
}*/

    if (ray->is_door)
    {
        door = find_door(game, ray->map_x, ray->map_y);
        if (door && door->state != DOOR_OPEN)
        {
            wall.texture = &game->door_system->door_texture;
            
            double wallx;
            if (ray->side == 0)
                wallx = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
            else
                wallx = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
            wallx -= floor(wallx);
            
            wall.tex.x = (int)(wallx * wall.texture->width);
            
            if (door->state == DOOR_OPENING || door->state == DOOR_CLOSING)
            {
                // Não invertemos mais o animation_progress
                double animation_progress = (door->state == DOOR_OPENING) ?
                                        door->animation : (1.0 - door->animation);
                
                // Usamos offset positivo
                int offset = (int)(wall.texture->width * animation_progress);
                wall.tex.x = (int)((wall.tex.x + offset) + wall.texture->width) % wall.texture->width;
            }
        }
    }
    // Renderização comum para todos os tipos de parede/porta
    pos.x = x;
    pos.y = wall.start - 1;
    while (++pos.y <= wall.end)
        put_wall_pixel(&wall, pos);
    buffer->y_top[x] = wall.end + 1;
    buffer->y_bottom[x] = wall.start - 1;
}
