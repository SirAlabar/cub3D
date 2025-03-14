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

void draw_portal_strip(t_wall *wall, t_portal_wall *portal)
{
    t_vector_i pos;
    t_texture *texture;
    int tex_x, tex_y;
    unsigned int color;
    double step, tex_pos;
    static int debug_counter = 0;
    
	if (wall->start < 0)
		wall->start = 0;
	if (wall->end >= WINDOW_HEIGHT)
		wall->end = WINDOW_HEIGHT - 1;
    /* Selecionar textura do portal */
	if (wall->end <= wall->start)
		return;
    texture = (portal->type == PORTAL_BLUE) ? 
        wall->game->portal_system->gun.blue_texture : 
        wall->game->portal_system->gun.orange_texture;
    
    /* Calcular coordenada x da textura do portal */
    tex_x = (int)(wall->pos.x * texture->width);
    if (wall->ray->side == 0 && wall->ray->dir.x > 0)
        tex_x = texture->width - tex_x - 1;
    if (wall->ray->side == 1 && wall->ray->dir.y > 0)
        tex_x = texture->width - tex_x - 1;
    
    /* Limitar a valores válidos */
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= texture->width) tex_x = texture->width - 1;
    
    /* Configurar passo de textura */
    step = 1.0 * texture->height / wall->height;
    tex_pos = (wall->start - (WINDOW_HEIGHT / 2) + (wall->height / 2)) * step;
    
    /* Iniciar posição de desenho */
    pos.x = wall->x;
    pos.y = wall->start - 1;
    
    if (debug_counter++ % 10000 == 0)
        printf("DEBUG: Portal strip at x=%d, tex_x=%d, start=%d, end=%d\n", 
            pos.x, tex_x, wall->start, wall->end);
    
    /* Loop para cada pixel vertical */
    while (++pos.y <= wall->end)
    {
        /* Calcular coordenada y da textura */
        tex_y = (int)tex_pos & (texture->height - 1);
        tex_pos += step;
        
        /* Obter cor do pixel na textura */
        color = get_texture_pixel(texture, tex_x, tex_y);
        
        /* Se o pixel for preto (transparente), não desenhar */
	/* Verificação mais rigorosa de transparência */
		if (color == 0x000001 || (color & 0xFFFFFF) == 0 || 
			((color >> 24) & 0xFF) == 0) // Verifica canal alfa se presente
			continue;
        
        /* Ajustar cor para sombreamento baseado na distância */
        color = apply_shade(color, 1.0 / (1.0 + wall->ray->perp_wall_dist * 0.05));
        
        /* Desenhar pixel na tela */
        draw_pixel(wall->game, pos.x, pos.y, color);
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
    
    /* Se o raio atingiu um portal */
    if (ray->hit_portal)
    {
        if (debug_counter++ % 10000 == 0)
            printf("DEBUG: Rendering portal strip at x=%d\n", x);
        
        /* Desenhar com verificação pixel a pixel */
        draw_portal_strip(&wall, ray->hit_portal);
    }
    else
    {
        /* Desenho normal de parede não-portal */
        draw_regular_wall(&wall);
    }
    
    /* Atualizar buffer de scanline */
    buffer->y_top[x] = wall.end + 1;
    buffer->y_bottom[x] = wall.start - 1;
}