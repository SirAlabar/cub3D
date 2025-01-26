/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:45:06 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/26 16:21:24 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

t_texture	*texture_create(t_game *game, char *texture_path)
{
	t_texture	*texture;

	texture = (t_texture *)ft_calloc(1, sizeof(t_texture));
	if (!texture)
	{
		ft_printf("Error: Failed to allocate memory for texture\n");
		return (NULL);
	}
	texture->img = mlx_xpm_file_to_image(game->mlx, texture_path,
			&texture->width, &texture->height);
	if (!texture->img)
	{
		ft_printf("Error: Failed to load texture: %s\n", texture_path);
		free(texture);
		return (NULL);
	}
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp,
			&texture->line_len, &texture->endian);
	return (texture);
}
/*
t_texture *get_wall_texture(t_ray *ray, t_game *game)
{
    char tile;
    t_door *door;

    tile = game->map.grid[ray->map_x][ray->map_y];
    
    if (tile == '1')
    {
        t_vector_i door_pos = {-1, -1};
        // Procura porta adjacente e guarda sua posição
        if (ray->map_y > 0 && game->map.grid[ray->map_x][ray->map_y - 1] == 'D')
            door_pos = (t_vector_i){ray->map_x, ray->map_y - 1};
        else if (ray->map_y < game->map.width - 1 && game->map.grid[ray->map_x][ray->map_y + 1] == 'D')
            door_pos = (t_vector_i){ray->map_x, ray->map_y + 1};
        else if (ray->map_x > 0 && game->map.grid[ray->map_x - 1][ray->map_y] == 'D')
            door_pos = (t_vector_i){ray->map_x - 1, ray->map_y};
        else if (ray->map_x < game->map.height - 1 && game->map.grid[ray->map_x + 1][ray->map_y] == 'D')
            door_pos = (t_vector_i){ray->map_x + 1, ray->map_y};

        if (door_pos.x != -1 && (door = find_door(game, door_pos.x, door_pos.y)))
        {
            // Verifica se estamos na frente da porta baseado na direção do raio
            int is_front_side = 0;
            
            if (door->orient == DOOR_VERTICAL)
            {
                if (ray->side == 0)  // Parede vertical
                {
                    // Verifica se o raio está vindo da direção da porta
                    if ((ray->dir.x > 0 && ray->map_x > door_pos.x) ||
                        (ray->dir.x < 0 && ray->map_x < door_pos.x))
                        is_front_side = 1;
                }
            }
            else  // DOOR_HORIZONTAL
            {
                if (ray->side == 1)  // Parede horizontal
                {
                    // Verifica se o raio está vindo da direção da porta
                    if ((ray->dir.y > 0 && ray->map_y > door_pos.y) ||
                        (ray->dir.y < 0 && ray->map_y < door_pos.y))
                        is_front_side = 1;
                }
            }

            if (is_front_side)
                return (&game->door_system->doorwall_texture);
        }
    }
	
    if (ray->is_door)
        return (&game->door_system->door_texture);
    // Texturas normais de parede
    if (ray->side == 0)
    {
        if (ray->dir.x > 0)
            return (&game->west);
        else
            return (&game->east);
    }
    else
    {
        if (ray->dir.y > 0)
            return (&game->north);
        else
            return (&game->south);
    }
}*/
/*
t_texture *get_wall_texture(t_ray *ray, t_game *game)
{
    char tile;
    t_door *door;
    
    tile = game->map.grid[ray->map_x][ray->map_y];
    
    if (tile == '1')
    {
        t_vector_i door_pos = {-1, -1};
        
        // Procura porta adjacente e guarda sua posição
        if (ray->map_y > 0 && game->map.grid[ray->map_x][ray->map_y - 1] == 'D')
            door_pos = (t_vector_i){ray->map_x, ray->map_y - 1};
        else if (ray->map_y < game->map.width - 1 && game->map.grid[ray->map_x][ray->map_y + 1] == 'D')
            door_pos = (t_vector_i){ray->map_x, ray->map_y + 1};
        else if (ray->map_x > 0 && game->map.grid[ray->map_x - 1][ray->map_y] == 'D')
            door_pos = (t_vector_i){ray->map_x - 1, ray->map_y};
        else if (ray->map_x < game->map.height - 1 && game->map.grid[ray->map_x + 1][ray->map_y] == 'D')
            door_pos = (t_vector_i){ray->map_x + 1, ray->map_y};

        if (door_pos.x != -1 && (door = find_door(game, door_pos.x, door_pos.y)))
        {
            int is_front_side = 0;

            // Porta vertical (orientada em Y)
            if (door->orient == DOOR_VERTICAL)
            {
                // Se a diferença está no eixo X (lado da porta)
                if (door_pos.x != ray->map_x && ray->side == 0)
                    is_front_side = 1;
            }
            // Porta horizontal (orientada em X)
            else
            {
                // Se a diferença está no eixo Y (lado da porta)
                if (door_pos.y != ray->map_y && ray->side == 1)
                    is_front_side = 1;
            }

            if (is_front_side)
                return (&game->door_system->doorwall_texture);
        }
    }

    if (ray->is_door)
        return (&game->door_system->door_texture);

    // Texturas normais de parede
    if (ray->side == 0)
    {
        if (ray->dir.x > 0)
            return (&game->west);
        else
            return (&game->east);
    }
    else
    {
        if (ray->dir.y > 0)
            return (&game->north);
        else
            return (&game->south);
    }
}*/

t_texture *get_wall_texture(t_ray *ray, t_game *game)
{
    char tile;
    t_door *door;

    // Obtém o tile atual do mapa
    tile = game->map.grid[ray->map_x][ray->map_y];

    // Verifica se o tile é uma porta
    if (tile == 'D')
    {
        door = find_door(game, ray->map_x, ray->map_y);
        if (door)
        {
            // Porta vertical
            if (door->orient == DOOR_VERTICAL && ray->side == 0)
                return (&game->door_system->doorwall_texture);

            // Porta horizontal
            if (door->orient == DOOR_HORIZONTAL && ray->side == 1)
                return (&game->door_system->doorwall_texture);

            // Retorna textura padrão da porta
            return (&game->door_system->door_texture);
        }
    }

    // Verifica se o tile é uma parede
    if (tile == '1')
    {
        // Identifica se há uma porta adjacente
        if ((ray->map_y > 0 && game->map.grid[ray->map_x][ray->map_y - 1] == 'D') || // Porta acima
            (ray->map_y < game->map.width - 1 && game->map.grid[ray->map_x][ray->map_y + 1] == 'D') || // Porta abaixo
            (ray->map_x > 0 && game->map.grid[ray->map_x - 1][ray->map_y] == 'D') || // Porta à esquerda
            (ray->map_x < game->map.height - 1 && game->map.grid[ray->map_x + 1][ray->map_y] == 'D')) // Porta à direita
        {
            // Retorna textura de moldura da porta
            return (&game->door_system->doorwall_texture);
        }

        // Decide textura de parede normal
        if (ray->side == 0)
        {
            if (ray->dir.x > 0)
                return (&game->west);
            else
                return (&game->east);
        }
        else
        {
            if (ray->dir.y > 0)
                return (&game->north);
            else
                return (&game->south);
        }
    }

    return NULL; // Caso nenhum tile válido seja encontrado
}




static t_vector_i	get_scale_coords(t_vector_i pos, double scale_x,
		double scale_y)
{
	t_vector_i	src;

	src.x = pos.x * scale_x;
	src.y = pos.y * scale_y;
	return (src);
}

static double	get_scale_factor(int src_size, int dst_size)
{
	return ((double)src_size / dst_size);
}

void	resize_texture(t_texture *src, t_texture *dst)
{
	t_vector_i	pos;
	t_vector_i	src_pos;
	double		scale_x;
	double		scale_y;
	int			color;

	scale_x = get_scale_factor(src->width, dst->width);
	scale_y = get_scale_factor(src->height, dst->height);
	pos.y = -1;
	while (++pos.y < dst->height)
	{
		pos.x = -1;
		while (++pos.x < dst->width)
		{
			src_pos = get_scale_coords(pos, scale_x, scale_y);
			color = get_texture_pixel(src, src_pos.x, src_pos.y);
			if ((color & 0xFFC0CB) == 0)
				continue ;
			draw_texture_pixel(dst, pos.x, pos.y, color);
		}
	}
}
