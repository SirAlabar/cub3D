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

void draw_portal_strip(t_game *game, t_ray *ray, int x)
{
    t_vector_i pos;
    t_texture *texture;
    unsigned int color;
    int tex_y;
    int start, end;
    t_portal_wall *portal;
    
    /* Verificar se o raio tem um hit_portal */
    if (!ray->hit_portal)
        return;
    
    portal = ray->hit_portal;
    
    /* Verificar se o portal está linkado */
    if (!portal->linked_portal || !portal->linked_portal->active)
        return;
    
    /* Calcular altura e limites da linha de portal usando os dados salvos */
    double saved_dist = ray->portal.perp_wall_dist;
    if (saved_dist < 0.5)
        saved_dist = 0.5;
    
    int line_height = (int)(WINDOW_HEIGHT / saved_dist);
    start = -line_height / 2 + WINDOW_HEIGHT / 2;
    end = line_height / 2 + WINDOW_HEIGHT / 2;
    
    /* Ajustar limites da tela */
    if (start < 0)
        start = 0;
    if (end >= WINDOW_HEIGHT)
        end = WINDOW_HEIGHT - 1;
    
    if (end <= start)
        return;
    
    /* Obter a textura do portal */
    texture = (portal->type == PORTAL_BLUE) ?
        game->portal_system->gun.blue_texture :
        game->portal_system->gun.orange_texture;
    
    /* Calcular coordenada x de textura para o portal */
    double wall_x;
    if (ray->portal.side == 0)
        wall_x = game->p1.pos.y + saved_dist * ray->dir.y;
    else
        wall_x = game->p1.pos.x + saved_dist * ray->dir.x;
    wall_x -= floor(wall_x);
    
    int tex_x = (int)(wall_x * texture->width);
    if (ray->portal.side == 0 && ray->dir.x < 0)
        tex_x = texture->width - tex_x - 1;
    if (ray->portal.side == 1 && ray->dir.y > 0)
        tex_x = texture->width - tex_x - 1;
    
    /* Desenhar a tira vertical */
    pos.x = x;
    pos.y = start - 1;
    
    /* Obter a textura da parede atual (após teleporte) */
    t_texture *current_texture = get_wall_texture(ray, game);
    
    while (++pos.y <= end)
    {
        tex_y = (pos.y - start) * texture->height / (end - start);
        if (tex_y < 0) tex_y = 0;
        if (tex_y >= texture->height) tex_y = texture->height - 1;
        
        color = get_texture_pixel(texture, tex_x, tex_y);
        
        /* Se a cor é transparente, desenhar o raio atual (após teleporte) */
        if (color == 0x000001)
        {
            /* Usar a textura e as informações do raio atual */
            if (current_texture)
            {
                /* Calcular a coordenada de textura para o raio atual */
                int current_tex_x = ray->tex_x;
                
                /* Calcular a coordenada y proporcionalmente */
                int line_pos = pos.y - start;
                int current_tex_y = (line_pos * ray->line_height) / line_height;
                current_tex_y = ray->draw_start + current_tex_y;
                
                /* Obter a cor da textura atual */
                unsigned int current_color = get_texture_pixel(current_texture, current_tex_x, current_tex_y);
                
                /* Desenhar o pixel com a cor da textura atual */
                draw_pixel(game, pos.x, pos.y, current_color);
            }
            else
            {
                /* Fallback para cores simples */
                if (portal->type == PORTAL_BLUE)
                    draw_pixel(game, pos.x, pos.y, 0x0000AA);
                else
                    draw_pixel(game, pos.x, pos.y, 0xAA5500);
            }
        }
        else if (color != 0xFFC0CB) /* Ignorar a cor rosa */
        {
            /* Desenhar a borda do portal */
            draw_pixel(game, pos.x, pos.y, apply_shade(color, 1.0 / (1.0 + saved_dist * 0.05)));
        }
    }
}

void render_portal_wall(t_ray *ray, t_game *game, int x, t_scanline *buffer)
{
    t_wall wall;
    t_portal_wall *portal;
    double perp_wall_dist;
    static int debug_counter = 0;
   
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Iniciando para raio x=%d, map_pos=(%d,%d)\n", 
        x, ray->map_x, ray->map_y);}
    
    portal = get_portal(ray, game);
    if (!portal || !portal->active)
    {
		if (debug_counter++ % 10000 == 0)
		{
        printf("DEBUG [render_portal_wall]: Nenhum portal ativo encontrado\n");}
        return;
    }
   
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Portal encontrado do tipo %s na posição (%d,%d)\n", 
        portal->type == PORTAL_BLUE ? "AZUL" : "LARANJA", 
        portal->position.x, portal->position.y);}
    
    perp_wall_dist = ray->portal.perp_wall_dist;
    // if (perp_wall_dist < 0.5)
    // {
    //     perp_wall_dist = 0.5;
    //     printf("DEBUG [render_portal_wall]: Ajustando perp_wall_dist para 0.5\n");
    // }
   
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: perp_wall_dist = %.2f\n", perp_wall_dist);}
    
    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Configurando textura do portal\n");}
    
    if (portal->type == PORTAL_BLUE)
    {
        wall.texture = game->portal_system->gun.blue_texture;
		if (debug_counter++ % 10000 == 0)
        {
        printf("DEBUG [render_portal_wall]: Usando textura do portal AZUL: %p\n", (void*)wall.texture);}
    }
    else if (portal->type == PORTAL_ORANGE)
    {
        wall.texture = game->portal_system->gun.orange_texture;
		if (debug_counter++ % 10000 == 0)
        {
        printf("DEBUG [render_portal_wall]: Usando textura do portal LARANJA: %p\n", (void*)wall.texture);}
    }
    else
    {
		if (debug_counter++ % 10000 == 0)
        {
        printf("DEBUG [render_portal_wall]: Tipo de portal desconhecido, retornando\n");}
        return;
    }
    
    wall.height = ray->line_height;
    wall.start = (-wall.height / 2) + (WINDOW_HEIGHT / 2);
    wall.end = (wall.height / 2) + (WINDOW_HEIGHT / 2);
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Dimensões da parede: height=%d, start=%d, end=%d\n", 
        wall.height, wall.start, wall.end);}
    
    set_wall_tex_coords(&wall);
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Coordenadas de textura: tex.x=%.2f\n", wall.tex.x);}
    
    wall.step = 1.0 * wall.texture->height / wall.height;
    wall.tex_pos = (wall.start - (WINDOW_HEIGHT / 2 - wall.height / 2)) * wall.step;
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Parâmetros de textura: step=%.2f, tex_pos=%.2f\n", 
        wall.step, wall.tex_pos);}
	if (debug_counter++ % 10000 == 0)
        {
    printf("DEBUG [render_portal_wall]: Chamando draw_portal_scanline\n");}
    draw_portal_scanline(game, &wall);
	if (debug_counter++ % 10000 == 0)
	{
    printf("DEBUG [render_portal_wall]: Finalizado para raio x=%d\n", x);}
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
		// draw_portal_strip(game, &rays[x], x);
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

void    draw_portal_gun(t_game *game)
{
    t_texture   *current_gun;
    t_vector_i  dim;
    t_vector_i  gun_pos;
    t_vector_i  pos;

    current_gun = &game->portal_system->gun.gun_anim[game->portal_system->gun.current_frame];
    if (!current_gun || !current_gun->img)
        return ;
    dim = get_weapon_dimensions(current_gun);
    gun_pos = get_weapon_position(dim);
    pos.y = gun_pos.y - 1;
    while (++pos.y < gun_pos.y + dim.y)
    {
        pos.x = gun_pos.x - 1;
        while (++pos.x < gun_pos.x + dim.x)
            draw_weapon_pixel(game, pos, gun_pos, current_gun);
    }
}
