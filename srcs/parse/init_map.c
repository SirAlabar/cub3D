/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 17:55:36 by hluiz-ma          #+#    #+#             */
/*   Updated: 2025/01/05 17:30:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_map(t_map *map)
{
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->floor_color = 0x87CEEB;
	map->ceiling_color = 0x8B4513;
	map->player_start = '\0';
	map->player_pos = vector_create(0, 0);
}
