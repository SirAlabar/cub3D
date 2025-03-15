/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_rendering.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:10:01 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/27 20:32:51 by hluiz-ma         ###   ########.fr       */
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
	wall->start = (-wall->height >> 1) + (WINDOW_HEIGHT >> 1);
	wall->end = (wall->height >> 1) + (WINDOW_HEIGHT >> 1);
	if (wall->start < wall->buffer->y_top[wall->x])
		wall->start = wall->buffer->y_top[wall->x];
	if (wall->end > wall->buffer->y_bottom[wall->x])
		wall->end = wall->buffer->y_bottom[wall->x];
	wall->texture = get_wall_texture(wall->ray, wall->game);
	set_wall_tex_coords(wall);
	wall->step = 1.0 * wall->texture->height / wall->height;
	wall->tex_pos = (wall->start - (WINDOW_HEIGHT >> 1) + (wall->height >> 1))
		* wall->step;
}

// void	set_wall_tex_coords(t_wall *wall)
// {
// 	if (wall->ray->side == 0)
// 		wall->pos.x = wall->game->p1.pos.y + wall->ray->perp_wall_dist
// 			* wall->ray->dir.y;
// 	else
// 		wall->pos.x = wall->game->p1.pos.x + wall->ray->perp_wall_dist
// 			* wall->ray->dir.x;
// 	wall->pos.x -= floor(wall->pos.x);
// 	wall->tex.x = (int)(wall->pos.x * wall->texture->width);
// 	if (wall->ray->is_door)
// 		adjust_door_texture(wall);
// 	if (wall->ray->side == 0 && wall->ray->dir.x < 0)
// 		wall->tex.x = wall->texture->width - wall->tex.x - 1;
// 	if (wall->ray->side == 1 && wall->ray->dir.y > 0)
// 		wall->tex.x = wall->texture->width - wall->tex.x - 1;
// }

void set_wall_tex_coords(t_wall *wall)
{
	static int debug_counter = 0;

    if (debug_counter++ % 60000 == 0){
		printf("DEBUG: Setting wall tex coords for pos (%d,%d)\n", 
			wall->ray->map_x, wall->ray->map_y);}
    
    if (wall->ray->side == 0)
        wall->pos.x = wall->game->p1.pos.y + wall->ray->perp_wall_dist
            * wall->ray->dir.y;
    else
        wall->pos.x = wall->game->p1.pos.x + wall->ray->perp_wall_dist
            * wall->ray->dir.x;
    
    wall->pos.x -= floor(wall->pos.x);
    wall->tex.x = (int)(wall->pos.x * wall->texture->width);
    
    if (debug_counter++ % 60000 == 0){
		printf("DEBUG: Wall tex.x = %f, pos.x = %.2f, texture width = %d\n", 
			wall->tex.x, wall->pos.x, wall->texture->width);}
    
    if (wall->ray->is_door)
        adjust_door_texture(wall);
    if (wall->ray->side == 0 && wall->ray->dir.x < 0)
        wall->tex.x = wall->texture->width - wall->tex.x - 1;
    if (wall->ray->side == 1 && wall->ray->dir.y > 0){
        wall->tex.x = wall->texture->width - wall->tex.x - 1;}
        
	if (debug_counter++ % 60000 == 0)
	{
		printf("DEBUG: Final wall tex.x = %f\n", wall->tex.x);
	}
}

static void	put_wall_pixel(t_wall *wall, t_vector_i pos)
{
	int	color;

	if (!wall->texture || !wall->texture->img
		|| !wall->game->addr[wall->game->current_buffer])
		return ;
	wall->tex.y = (int)wall->tex_pos & (wall->texture->height - 1);
	color = get_texture_pixel(wall->texture, wall->tex.x, wall->tex.y);
	color = apply_shade(color, 1.0 / (1.0 + wall->ray->perp_wall_dist * 0.05));
	draw_pixel(wall->game, pos.x, pos.y, color);
	wall->tex_pos += wall->step;
}
void draw_regular_wall(t_wall *wall)
{
    t_vector_i pos;
    
    pos.x = wall->x;
    pos.y = wall->start - 1;
    
    while (++pos.y <= wall->end)
        put_wall_pixel(wall, pos);
}

void draw_portal_strip(t_game *game, t_wall *wall, t_portal_wall *portal)
{
    t_vector_i pos;
    t_texture *texture;
    unsigned int color;
    int tex_y;
    // double y_ratio;
   
    // Limites de segurança
    if (wall->start < 0)
        wall->start = 0;
    if (wall->end >= WINDOW_HEIGHT)
        wall->end = WINDOW_HEIGHT - 1;
   
    if (wall->end <= wall->start)
        return;
    // Obter a textura do portal que já foi calculada
    texture = (portal->type == PORTAL_BLUE) ?
        game->portal_system->gun.blue_texture :
        game->portal_system->gun.orange_texture;
   
    // Para cada pixel vertical
    pos.x = wall->x;
    pos.y = wall->start - 1;
   
    while (++pos.y <= wall->end)
    {
        // Mapear coordenadas da tela para coordenadas da textura
        // y_ratio = (double)(pos.y - wall->start) / (double)(wall->end - wall->start);
        tex_y = texture->height;
       
        // Limitar a valores válidos
        if (tex_y < 0) tex_y = 0;
        if (tex_y >= texture->height) tex_y = texture->height - 1;
       
        // Obter a cor do pixel da textura
        color = get_texture_pixel(texture, (int)wall->tex.x, tex_y);
       
        // Se não for o pixel transparente específico (0x000001), então desenha
        // if (color == 0x000001)
		if (color == 0x000001)
        {
            draw_pixel(game, pos.x, pos.y, color);
        }
        // Se for 0x000001, não fazemos nada - já está renderizado pelo raycasting
    }
}

// void	draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
// {
// 	t_wall		wall;


// 	if (!game || !game->addr[game->current_buffer] || !ray || !buffer)
// 		return ;
// 	wall.game = game;
// 	wall.ray = ray;
// 	wall.x = x;
// 	wall.buffer = buffer;
// 	init_wall_drawing(&wall);
//     if (ray->hit_portal)
//     {
//         /* Desenhar com verificação pixel a pixel */
//         draw_portal_strip(&wall, ray->hit_portal);
//     }
//     else
//     {
//         /* Desenho normal de parede não-portal */
//         draw_regular_wall(&wall);
//     }
// 	buffer->y_top[x] = wall.end + 1;
// 	buffer->y_bottom[x] = wall.start - 1;
// }

void draw_wall_scanline(t_game *game, t_ray *ray, int x, t_scanline *buffer)
{
    t_wall wall;
    static int debug_counter = 0;
    
    if (!game || !ray || !buffer)
        return;
        
    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
    
    /* Inicializar parede para desenho */
    init_wall_drawing(&wall);
	if (debug_counter++ % 10000 == 0)
	printf("DEBUG: Drawing regular wall at x=%d for position (%d,%d)\n", 
		x, ray->map_x, ray->map_y);
	draw_regular_wall(&wall);
    /* Se o raio atingiu um portal */
    if (ray->hit_portal)
    {
        if (debug_counter++ % 10000 == 0)
		printf("DEBUG: Overlaying portal at x=%d, type=%s\n", x,
			ray->hit_portal->type == PORTAL_BLUE ? "BLUE" : "ORANGE");
        
        /* Desenhar com verificação pixel a pixel */
        draw_portal_strip(game, &wall, ray->hit_portal);
    }



    
    /* Atualizar buffer de scanline */
    buffer->y_top[x] = wall.end + 1;
    buffer->y_bottom[x] = wall.start - 1;
}