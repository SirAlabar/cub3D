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

// void	render_portal_wall(t_ray *ray, t_game *game, int x)
// {
// 	t_portal_wall	*portal;
// 	double			tex_x;
// 	double			tex_y;
// 	int				y;
// 	unsigned int	color;

// 	portal = get_portal(ray, game);
// 	if (!portal)
// 		return ;
// 	if (ray->side == 0)
// 		tex_x = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
// 	else
// 		tex_x = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
// 	tex_x -= floor(tex_x);
// 	y = ray->draw_start;
// 	while (y < ray->draw_end)
// 	{
// 		tex_y = (double)(y - ray->draw_start) / (ray->draw_end - ray->draw_start);
//         color = get_portal_color(portal, tex_x, tex_y, game);
// 		if (color != 0xFFC0CB)
// 			draw_pixel(game, x, y, color);
// 		y++;
// 	}
// }

void render_portal_wall(t_ray *ray, t_game *game, int x)
{
    t_portal_wall *portal;
    double wallx;
    int tex_x, tex_y;
    int y;
    unsigned int color;
    t_texture *texture;
    
    portal = get_portal(ray, game);
    if (!portal || !portal->active)
        return;
    if (ray->perp_wall_dist < 0.5)
        return;
    
    // Verificar se estamos dentro dos limites da tela
    if (x < 0 || x >= WINDOW_WIDTH)
        return;
    
    // Selecionar a textura correta
    if (portal->type == PORTAL_BLUE)
        texture = game->portal_system->gun.blue_texture;
    else if (portal->type == PORTAL_ORANGE)
        texture = game->portal_system->gun.orange_texture;
    else
        return; // Tipo inválido
    
    // Calcular a posição exata na parede
    if (ray->side == 0)
        wallx = game->p1.pos.y + ray->perp_wall_dist * ray->dir.y;
    else
        wallx = game->p1.pos.x + ray->perp_wall_dist * ray->dir.x;
    wallx -= floor(wallx);
    
    // Converter para coordenada de textura
    tex_x = (int)(wallx * texture->width);
    
    // Verificar limites
    if (tex_x < 0 || tex_x >= texture->width)
        return;
    
    // Verificar limites de desenho
    if (ray->draw_start >= WINDOW_HEIGHT || ray->draw_end < 0)
        return;
    
    // Ajustar limites de Y
    int start_y = ray->draw_start < 0 ? 0 : ray->draw_start;
    int end_y = ray->draw_end >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : ray->draw_end;
    
    // Desenhar a coluna de pixels
    y = start_y;
    while (y <= end_y)
    {
        // Calcular coordenada Y na textura
        tex_y = (int)((double)(y - ray->draw_start) / 
                      (ray->draw_end - ray->draw_start) * texture->height);
        
        // Verificar limites Y
        if (tex_y < 0 || tex_y >= texture->height)
        {
            y++;
            continue;
        }
        
        // Obter a cor do pixel
        color = get_texture_pixel(texture, tex_x, tex_y);
        
        // Desenhar se não for a cor transparente
        if (color != 0xFFC0CB)
            draw_pixel(game, x, y, color);
        
        y++;
    }
}

void render_portals(t_game *game, t_ray *rays)
{
    int x;
    
    printf("DEBUG: Rendering portals - Blue active: %s, Orange active: %s\n", 
           game->portal_system->blue_portal.active ? "YES" : "NO",
           game->portal_system->orange_portal.active ? "YES" : "NO");
    if (!game->portal_system || (!game->portal_system->blue_portal.active && 
            !game->portal_system->orange_portal.active))
        return;
    
    x = 0;
    while (x < WINDOW_WIDTH)
    {
        render_portal_wall(&rays[x], game, x);
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
