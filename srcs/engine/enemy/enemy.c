/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 19:40:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/22 19:47:36 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	vector_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

static t_texture	*load_enemy_texture(t_game *game)
{
	t_texture	*texture;
	char		*path;

	path = "./assets/sprites/enemies/cacodemon00.xpm";
	texture = texture_create(game, path);
	if (!texture)
	{
		ft_printf("Error: Failed to load enemy texture: %s\n", path);
		return (NULL);
	}
	return (texture);
}

void	add_enemy(t_game *game, t_vector pos)
{
	t_enemy_list	*new;
	t_enemy_list	*temp;

	new = (t_enemy_list *)malloc(sizeof(t_enemy_list));
	if (!new)
		return ;
	new->enemy.pos = pos;
	new->enemy.health = 100;
	new->enemy.alive = true;
	new->enemy.dir = vector_create(0, 0);
	new->enemy.texture = load_enemy_texture(game);
	new->next = NULL;
	if (!game->enemies)
		game->enemies = new;
	else
	{
		temp = game->enemies;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

static void	calculate_enemy_distance(t_game *game, t_enemy *enemy)
{
	enemy->dist_to_player = vector_dist(game->p1.pos, enemy->pos);
	enemy->dir = vector_create(1, 0);
}

void	update_enemies(t_game *game)
{
	t_enemy_list	*current;

	current = game->enemies;
	while (current)
	{
		if (current->enemy.alive)
			calculate_enemy_distance(game, &current->enemy);
		current = current->next;
	}
}

static void	draw_enemy_stripe(t_game *game, t_enemy *enemy,
				t_sprite_data *s, int stripe)
{
	int	y;
	int	d;
	int	tex_y;
	int	color;
	int	tex_x;

	tex_x = (int)(256 * (stripe - (-s->width / 2 + s->screen_x))
			* enemy->texture->width / s->width) / 256;
	if (tex_x >= 0 && tex_x < enemy->texture->width)
	{
		y = s->start_y;
		while (y < s->end_y)
		{
			d = (y) * 256 - WINDOW_HEIGHT * 128 + s->height * 128;
			tex_y = ((d * enemy->texture->height) / s->height) / 256;
			if (tex_y >= 0 && tex_y < enemy->texture->height)
			{
				color = get_texture_pixel(enemy->texture, tex_x, tex_y);
				if ((color & 0x00FFFFFF) != 0)
					draw_pixel(game, stripe, y,
						apply_shade(color, 1.0 / (1.0
								+ enemy->dist_to_player * 0.04)));
			}
			y++;
		}
	}
}

static void	draw_enemy_sprite(t_game *game, t_enemy *enemy,
				double screen_x, int sprite_height)
{
	t_sprite_data	s;
	int				stripe;

	s.height = sprite_height;
	s.width = sprite_height;
	s.screen_x = screen_x;
	s.start_y = WINDOW_HEIGHT / 2 - sprite_height / 2;
	s.end_y = WINDOW_HEIGHT / 2 + sprite_height / 2;
	s.start_x = screen_x - s.width / 2;
	s.end_x = screen_x + s.width / 2;
	if (s.start_y < 0)
		s.start_y = 0;
	if (s.end_y >= WINDOW_HEIGHT)
		s.end_y = WINDOW_HEIGHT - 1;
	if (s.start_x < 0)
		s.start_x = 0;
	if (s.end_x >= WINDOW_WIDTH)
		s.end_x = WINDOW_WIDTH - 1;
	stripe = s.start_x;
	while (stripe < s.end_x)
	{
		draw_enemy_stripe(game, enemy, &s, stripe);
		stripe++;
	}
}

static void	calculate_ray_step(t_ray_data *ray, t_vector ray_dir)
{
	if (ray_dir.x < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (ray->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - ray->pos.x) * ray->delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (ray->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - ray->pos.y) * ray->delta_dist.y;
	}
}

static bool	is_enemy_visible(t_game *game, t_vector enemy_pos)
{
	t_ray_data	ray;
	t_vector	ray_dir;

	ray_dir = vector_normalize(vector_sub(enemy_pos, game->p1.pos));
	ray.pos = game->p1.pos;
	ray.map_x = (int)ray.pos.x;
	ray.map_y = (int)ray.pos.y;
	ray.delta_dist.x = fabs(1 / ray_dir.x + 0.3);
	ray.delta_dist.y = fabs(1 / ray_dir.y);
	calculate_ray_step(&ray, ray_dir);
	while (1)
	{
		if (ray.side_dist.x < ray.side_dist.y)
		{
			ray.side_dist.x += ray.delta_dist.x;
			ray.map_x += ray.step.x;
		}
		else
		{
			ray.side_dist.y += ray.delta_dist.y;
			ray.map_y += ray.step.y;
		}
		if (ray.map_x == (int)enemy_pos.x && ray.map_y == (int)enemy_pos.y)
			return (true);
		if (game->map.grid[ray.map_x][ray.map_y] == '1')
			return (false);
	}
}

void	draw_enemies(t_game *game)
{
	t_enemy_list	*current;
	t_vector		rel_pos;
	double			angle;
	double			fov;
	double			dist;

	current = game->enemies;
	fov = 2 * atan2(vector_length(game->p1.plane), 1.0);
	while (current && current->enemy.alive)
	{
		if (!is_enemy_visible(game, current->enemy.pos))
		{
			current = current->next;
			continue ;
		}
		rel_pos = vector_sub(current->enemy.pos, game->p1.pos);
		dist = vector_dist(game->p1.pos, current->enemy.pos);
		angle = atan2(rel_pos.y, rel_pos.x)
			- atan2(game->p1.dir.y, game->p1.dir.x);
		if (angle < -M_PI)
			angle += 2 * M_PI;
		if (angle > M_PI)
			angle -= 2 * M_PI;
		if (fabs(angle) < fov / 2)
			draw_enemy_sprite(game, &current->enemy,
				(angle + fov / 2) * (WINDOW_WIDTH / fov),
				(int)(WINDOW_HEIGHT / dist));
		current = current->next;
	}
}

static void	spawn_enemies_from_map(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'E')
			{
				add_enemy(game, vector_create(j + 0.5, i + 0.5));
				game->map.grid[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}

void	init_enemies(t_game *game)
{
	game->enemies = NULL;
	spawn_enemies_from_map(game);
}
