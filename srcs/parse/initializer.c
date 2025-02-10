/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:02:20 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/02/10 21:07:14 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void init_player(t_game *game, t_thing *player_thing)
{
    t_player *p = &game->p1;
    p->pos.x = player_thing->pos.x;
    p->pos.y = player_thing->pos.y;

    p->angle = player_thing->angle;
    p->dir.x = fixed32_cos(p->angle);
    p->dir.y = fixed32_sin(p->angle);
    p->plane.x = fixed32_mul(p->dir.y, int_to_fixed32(2));
    p->plane.y = fixed32_mul(-p->dir.x, int_to_fixed32(2));
    p->move_speed = float_to_fixed32(MOVE_SPEED);
    p->rot_speed = float_to_fixed32(ROTATION_SPEED);
    p->current_frame = 0;
    p->is_firing = 0;
    p->last_fire = 0;
    p->health = 100;
    p->sector = find_player_sector(game->map);
    ft_bzero(&p->keys, sizeof(t_keys));
}

void init_textures(t_game *game)
{
    game->p1.gun_anim = ft_calloc(1, (sizeof(t_texture) * 4));
    if (!game->p1.gun_anim)
        return;
    game->p1.gun_anim[0] = *texture_create(game, GUN_F1);
    game->p1.gun_anim[1] = *texture_create(game, GUN_F2);
    game->p1.gun_anim[2] = *texture_create(game, GUN_F3);
    game->p1.gun_anim[3] = *texture_create(game, GUN_F4);
}

void init_game(t_game *game)
{
    init_double_buffer(game);
    init_player(game);
    init_textures(game);
    game->mouse_sensi = 0.003;
    game->last_mouse = vector_create(-1, -1);
    game->fps = 0;
}