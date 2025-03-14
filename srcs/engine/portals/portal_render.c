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


void render_portal_wall(t_ray *ray, t_game *game, int x, t_scanline *buffer)
{
    t_wall wall;
    t_portal_wall *portal;
	double perp_wall_dist;
	static int debug_counter = 0;
    
    portal = get_portal(ray, game);
    if (!portal || !portal->active)
	{
        return;
	}
    
	if (debug_counter++ % 3000 == 0)
		printf("DEBUG: Rendering portal at x=%d (%d,%d) - line_height=%d, start=%d, end=%d\n", 
			x, ray->map_x, ray->map_y, ray->line_height, ray->draw_start, ray->draw_end);
		
	perp_wall_dist = ray->perp_wall_dist;
	if (perp_wall_dist < 0.5)
		perp_wall_dist = 0.5;
	 
    // Usar a mesma altura e posição que a parede teria
    wall.game = game;
    wall.ray = ray;
    wall.x = x;
    wall.buffer = buffer;
    
    // Selecionar a textura correta
    if (portal->type == PORTAL_BLUE)
        wall.texture = game->portal_system->gun.blue_texture;
    else if (portal->type == PORTAL_ORANGE)
        wall.texture = game->portal_system->gun.orange_texture;
    else
	{
        return;
	}
	
	// if (debug_counter++ % 3000 == 0){
	// 	printf("DEBUG: Portal texture size: %dx%d\n", wall.texture->width, wall.texture->height);
	// 	printf("DEBUG: Scanline buffer limits for x=%d: top=%d, bottom=%d\n", 
	// 			x, buffer->y_top[x], buffer->y_bottom[x]);}

    // Aqui está a parte importante: usar as mesmas dimensões da parede
    // wall.height = ray->line_height;
    // wall.start = ray->draw_start;
    // wall.end = ray->draw_end;
    wall.height = ray->line_height;
    wall.start = (-wall.height / 2) + (WINDOW_HEIGHT / 2);
    wall.end = (wall.height / 2) + (WINDOW_HEIGHT / 2);
    // // Respeitar os limites da tela
    // if (wall.start < 0)
    //     wall.start = 0;
    // if (wall.end >= WINDOW_HEIGHT)
	// {
    //     wall.end = WINDOW_HEIGHT - 1;
	// }
    
	// if (debug_counter++ % 3000 == 0)
	// 	printf("DEBUG: Final portal dimensions: height=%d, start=%d, end=%d\n", 
	// 		wall.height, wall.start, wall.end);
    // Calcular as coordenadas de textura exatamente como faria para a parede
    set_wall_tex_coords(&wall);
    
    // Cálculo do passo de textura com base na altura real da parede
    // wall.step = 1.0 * wall.texture->height / wall.height;
	// wall.tex_pos = (wall.start - (WINDOW_HEIGHT >> 1) + (wall.height >> 1))
	// 	* wall.step;
    wall.step = 1.0 * wall.texture->height / wall.height;
    wall.tex_pos = (wall.start - (WINDOW_HEIGHT / 2 - wall.height / 2)) * wall.step;

	// if (debug_counter++ % 3000 == 0)
	// 	printf("++++++++DEBUG: Rendering portal at x=%d (%d,%d) - line_height=%d, start=%d, end=%d\n", 
	// 		x, ray->map_x, ray->map_y, ray->line_height, ray->draw_start, ray->draw_end);
    draw_portal_scanline(game, &wall);
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
