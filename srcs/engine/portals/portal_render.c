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
void	render_portal_wall(t_ray *ray, t_game *game, int x, t_scanline *buffer)
{
	t_wall			wall;
	t_portal_wall	*portal;

	portal = get_portal(ray, game);
	if (!portal || !portal->active)
		return ;
	wall.game = game;
	wall.ray = ray;
	wall.x = x;
	wall.buffer = buffer;
	if (portal->type == PORTAL_BLUE)
		wall.texture = game->portal_system->gun.blue_texture;
	else if (portal->type == PORTAL_ORANGE)
		wall.texture = game->portal_system->gun.orange_texture;
	else
		return ;
	wall.height = ray->line_height;
	wall.start = (-wall.height / 2) + (WINDOW_HEIGHT / 2);
	wall.end = (wall.height / 2) + (WINDOW_HEIGHT / 2);
	set_wall_tex_coords(&wall);
	wall.step = 1.0 * wall.texture->height / wall.height;
	wall.tex_pos = (wall.start - (WINDOW_HEIGHT / 2 - wall.height / 2)) * wall.step;
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
