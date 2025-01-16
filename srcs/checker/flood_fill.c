/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 14:01:02 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/12 17:56:15 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static bool	recursive_flood_fill(char **grid, t_vector_i st, int h, int w)
{
	int		current_line_length;

	if (st.x < 0 || st.y < 0 || st.x >= h || st.y >= w
		|| grid[st.x][st.y] == '1' || grid[st.x][st.y] == 'X')
		return (true);
	current_line_length = 0;
	while (grid[st.x][current_line_length] != '\0')
		current_line_length++;
	if (st.y >= current_line_length)
		return (false);
	if (grid[st.x][st.y] == ' ')
		return (false);
	if (grid[st.x][st.y] == '0' && (st.x == 0 || st.y == 0
			|| st.x == h - 1 || st.y >= current_line_length - 1))
		return (false);
	grid[st.x][st.y] = 'X';
	if (!recursive_flood_fill(grid, (t_vector_i){st.x - 1, st.y}, h, w))
		return (false);
	if (!recursive_flood_fill(grid, (t_vector_i){st.x + 1, st.y}, h, w))
		return (false);
	if (!recursive_flood_fill(grid, (t_vector_i){st.x, st.y - 1}, h, w))
		return (false);
	if (!recursive_flood_fill(grid, (t_vector_i){st.x, st.y + 1}, h, w))
		return (false);
	return (true);
}

static bool	find_start_position(char **grid, int height,
		int width, t_vector_i *start)
{
	int	i;
	int	j;
	int	current_line_length;

	i = -1;
	while (++i < height)
	{
		j = -1;
		while (++j < width)
		{
			current_line_length = 0;
			while (grid[i][current_line_length] != '\0')
				current_line_length++;
			if (j >= current_line_length)
				continue ;
			if (grid[i][j] == 'N' || grid[i][j] == 'S'
				|| grid[i][j] == 'E' || grid[i][j] == 'W')
			{
				start->x = i;
				start->y = j;
				return (true);
			}
		}
	}
	return (false);
}

bool	flood_fill(char **grid, int height, int width, t_vector_i start)
{
	if (!recursive_flood_fill(grid, start, height, width))
	{
		printf("Error: Map is not fully enclosed.\n");
		return (false);
	}
	return (true);
}

static void	free_map_copy(char **copy, int height)
{
	int	i;

	i = -1;
	while (++i < height)
		free(copy[i]);
	free(copy);
}

bool	is_map_valid(t_game *game)
{
	char		**map_copy;
	t_vector_i	start;

	map_copy = copy_map(game->map.grid, game->map.height);
	if (!map_copy)
	{
		printf("Error:\n Failed to create map copy.\n");
		return (cleanup_game(game), exit(1), false);
	}
	if (!find_start_position(map_copy,
			game->map.height, game->map.width, &start))
	{
		printf("Error:\n Player starting position not found.\n");
		free_map_copy(map_copy, game->map.height);
		return (cleanup_game(game), exit(1), false);
	}
	if (!flood_fill(map_copy, game->map.height, game->map.width, start))
	{
		free_map_copy(map_copy, game->map.height);
		return (cleanup_game(game), exit(1), false);
	}
	game->p1.pos.x = (double) start.x;
	game->p1.pos.y = (double) start.y;
	free_map_copy(map_copy, game->map.height);
	return (true);
}
