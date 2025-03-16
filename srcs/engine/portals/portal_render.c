/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:00:00 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/03/07 14:30:00 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static t_portal_wall	*get_portal(t_ray *ray, t_game *game)
{
	if (check_portal_hit(ray, game, &game->portal_system->blue_portal))
		return (&game->portal_system->blue_portal);
	if (check_portal_hit(ray, game, &game->portal_system->orange_portal))
		return (&game->portal_system->orange_portal);
	return (NULL);
}


static void draw_portal_scanline(t_game *game, t_wall *wall)
{
    t_vector_i pos;
    unsigned int color;
    double y_ratio;
    int tex_y;
    
	(void)game;
    if (!wall->texture || !wall->texture->img || !wall->game->addr[wall->game->current_buffer])
        return;
    
    if (wall->end <= wall->start)
        return;
    pos.x = wall->x;
    pos.y = wall->start - 1;
    while (++pos.y <= wall->end)
    {
        y_ratio = (double)(pos.y - wall->start) / (double)(wall->end - wall->start);
        tex_y = (int)(y_ratio * wall->texture->height);
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= wall->texture->height)
            tex_y = wall->texture->height - 1;
        color = get_texture_pixel(wall->texture, (int)wall->tex.x, tex_y);
        if (color != 0xFFC0CB)
            draw_pixel(wall->game, pos.x, pos.y, color);
    }
}

void draw_portal_strip(t_game *game, t_wall *wall, t_portal_wall *portal)
{
    t_vector_i pos;
    t_texture *texture;
    unsigned int color;
    int tex_y;
   
    printf("DEBUG [draw_portal_strip]: Iniciando com wall start=%d, end=%d\n", wall->start, wall->end);
    
    /* Verificar se o portal está linkado */
    if (!portal->linked_portal || !portal->linked_portal->active)
    {
        printf("DEBUG [draw_portal_strip]: Portal não está linkado, retornando\n");
        return;
    }
    
    if (wall->start < 0)
        wall->start = 0;
    if (wall->end >= WINDOW_HEIGHT)
        wall->end = WINDOW_HEIGHT - 1;
   
    if (wall->end <= wall->start)
    {
        printf("DEBUG [draw_portal_strip]: wall->end <= wall->start, retornando\n");
        return;
    }
   
    texture = (portal->type == PORTAL_BLUE) ?
        game->portal_system->gun.blue_texture :
        game->portal_system->gun.orange_texture;
   
    printf("DEBUG [draw_portal_strip]: Textura do portal: %p, size=%dx%d\n",
        (void*)texture, texture->width, texture->height);
    
    pos.x = wall->x;
    pos.y = wall->start - 1;
   
    printf("DEBUG [draw_portal_strip]: Iniciando loop com pos.x=%d, pos.y=%d\n", pos.x, pos.y);
   
    int pixels_processed = 0;
    int transparent_pixels = 0;
   
    while (++pos.y <= wall->end)
    {
        tex_y = (pos.y - wall->start) * texture->height / (wall->end - wall->start);
        if (tex_y < 0) tex_y = 0;
        if (tex_y >= texture->height) tex_y = texture->height - 1;
       
        color = get_texture_pixel(texture, (int)wall->tex.x, tex_y);
       
        pixels_processed++;
        if (color == 0x000001)
        {
            transparent_pixels++;
            
            /* Verificar se temos informações do outro lado do portal */
            if (wall->ray->hit_portal && wall->ray->portal_depth > 0)
            {
                /* Calcular o que desenhar do outro lado do portal */
                /* Usar os dados armazenados em wall->ray->portal */
                
                /* Por agora, apenas desenhar uma cor diferente para garantir que
                   estamos identificando corretamente os pixels transparentes */
                draw_pixel(game, pos.x, pos.y, 0x0000FF); /* Azul */
            }
            else
            {
                /* Se não tivermos dados do outro lado, usar alguma cor de placeholder */
                draw_pixel(game, pos.x, pos.y, 0xFF0000); /* Vermelho */
            }
            
            if (pixels_processed % 50 == 0)
            {
                printf("DEBUG [draw_portal_strip]: Desenhando pixel transparente em (%d,%d), tex_y=%d\n",
                    pos.x, pos.y, tex_y);
            }
        }
    }
   
    printf("DEBUG [draw_portal_strip]: Loop concluído. Processados %d pixels, %d transparentes\n",
        pixels_processed, transparent_pixels);
    printf("DEBUG [draw_portal_strip]: Última posição: pos.x=%d, pos.y=%d\n", pos.x, pos.y);
}

void render_portal_wall(t_ray *ray, t_game *game, int x, t_scanline *buffer)
{
    t_wall wall;
    t_portal_wall *portal;
    double perp_wall_dist;
    // static int debug_counter = 0;
   
    printf("DEBUG [render_portal_wall]: Iniciando para raio x=%d, map_pos=(%d,%d)\n", 
        x, ray->map_x, ray->map_y);
    
    portal = get_portal(ray, game);
    if (!portal || !portal->active)
    {
        printf("DEBUG [render_portal_wall]: Nenhum portal ativo encontrado\n");
        return;
    }
   
    printf("DEBUG [render_portal_wall]: Portal encontrado do tipo %s na posição (%d,%d)\n", 
        portal->type == PORTAL_BLUE ? "AZUL" : "LARANJA", 
        portal->position.x, portal->position.y);
    
    perp_wall_dist = ray->perp_wall_dist;
    if (perp_wall_dist < 0.5)
    {
        perp_wall_dist = 0.5;
        printf("DEBUG [render_portal_wall]: Ajustando perp_wall_dist para 0.5\n");
    }
   
    printf("DEBUG [render_portal_wall]: perp_wall_dist = %.2f\n", perp_wall_dist);
    
    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
    
    printf("DEBUG [render_portal_wall]: Configurando textura do portal\n");
    
    if (portal->type == PORTAL_BLUE)
    {
        wall.texture = game->portal_system->gun.blue_texture;
        printf("DEBUG [render_portal_wall]: Usando textura do portal AZUL: %p\n", (void*)wall.texture);
    }
    else if (portal->type == PORTAL_ORANGE)
    {
        wall.texture = game->portal_system->gun.orange_texture;
        printf("DEBUG [render_portal_wall]: Usando textura do portal LARANJA: %p\n", (void*)wall.texture);
    }
    else
    {
        printf("DEBUG [render_portal_wall]: Tipo de portal desconhecido, retornando\n");
        return;
    }
    
    wall.height = ray->line_height;
    wall.start = (-wall.height / 2) + (WINDOW_HEIGHT / 2);
    wall.end = (wall.height / 2) + (WINDOW_HEIGHT / 2);
    
    printf("DEBUG [render_portal_wall]: Dimensões da parede: height=%d, start=%d, end=%d\n", 
        wall.height, wall.start, wall.end);
    
    set_wall_tex_coords(&wall);
    
    printf("DEBUG [render_portal_wall]: Coordenadas de textura: tex.x=%.2f\n", wall.tex.x);
    
    wall.step = 1.0 * wall.texture->height / wall.height;
    wall.tex_pos = (wall.start - (WINDOW_HEIGHT / 2 - wall.height / 2)) * wall.step;
    
    printf("DEBUG [render_portal_wall]: Parâmetros de textura: step=%.2f, tex_pos=%.2f\n", 
        wall.step, wall.tex_pos);
    
    printf("DEBUG [render_portal_wall]: Chamando draw_portal_scanline\n");
    draw_portal_scanline(game, &wall);
    
    printf("DEBUG [render_portal_wall]: Chamando draw_portal_strip\n");
    draw_portal_strip(game, &wall, portal);
    
    printf("DEBUG [render_portal_wall]: Finalizado para raio x=%d\n", x);
}



void render_portals(t_game *game, t_ray *rays, t_scanline *buffer)
{
    int x;

    if (!game->portal_system || (!game->portal_system->blue_portal.active
            && !game->portal_system->orange_portal.active))
        return;
    x = 0;
    while (x < WINDOW_WIDTH)
    {
        render_portal_wall(&rays[x], game, x, buffer);
        x++;
    }
}


// void	draw_portal_gun(t_game *game)
// {
// 	t_texture	*gun;
// 	t_vector_i	dim;
// 	t_vector_i	pos;
// 	t_vector_i	px;
// 	int			color;
	
// 	gun = game->portal_system->gun.gun_texture;
// 	if (!gun || !gun->img)
// 		return ;
// 	dim.x = gun->width / 2;
// 	dim.y = gun->height / 2;
// 	pos.x = WINDOW_WIDTH - dim.x - 100;
// 	pos.y = WINDOW_HEIGHT - dim.y - 50;
// 	px.y = 0;
// 	while (px.y < dim.y)
// 	{
// 		px.x = 0;
// 		while (px.x < dim.x)
// 		{
// 			color = get_texture_pixel(gun, px.x * 2, px.y * 2);
// 			if ((color & 0xFFC0CB) != 0)
// 				draw_pixel(game, pos.x + px.x, pos.y + px.y, color);
// 			px.x++;
// 		}
// 		px.y++;
// 	}
// }

void	draw_portal_gun(t_game *game)
{
	t_texture	*gun;
	t_vector_i	dim;
	t_vector_i	gun_pos;
	t_vector_i	pos;

	gun = game->portal_system->gun.gun_texture;
	if (!gun || !gun->img)
		return ;
	dim = get_weapon_dimensions(gun);
	gun_pos = get_weapon_position(dim);
	pos.y = gun_pos.y - 1;
	while (++pos.y < gun_pos.y + dim.y)
	{
		pos.x = gun_pos.x - 1;
		while (++pos.x < gun_pos.x + dim.x)
			draw_weapon_pixel(game, pos, gun_pos, gun);
	}
}
