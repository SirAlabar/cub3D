#include <cub3d.h>

void	*validate_player_count(t_game *game);

void	init_map(t_game *game)
{
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
	game->map.floor_color = 9134906;
	game->map.ceiling_color = 11272191;
	init_colors(game);
	game->map.player_start = '\0';
	game->map.player_pos = vector_create(0, 0);
	read_map(game);
	validate_player_count(game);
}

int	get_map_width(t_game *game)
{
	int	width;
	int	tmp;
	int	height;
	int	i;

	width = 0;
	height = game->map.height;
	i = 0;
	tmp = 0;
	while (height)
	{
		if (game->map.grid[i])
			tmp = ft_strlen(game->map.grid[i]);
		if (tmp > width)
			width = tmp;
		height--;
		i++;
	}
	return (width);
}

char	**read_map(t_game *game)
{
	game->map.height = count_lines(game);
	if (game->map.height <= 0)
		return (NULL);
	game->map.grid = ft_calloc(sizeof(char *), game->map.height + 1);
	if (!game->map.grid)
		return (NULL);
	set_grid(game);
	game->map.width = get_map_width(game);
	return (game->map.grid);
}

void	set_grid(t_game *game)
{
	char	*line;
	int		i;

	game->fd_map = open(game->map_path, O_RDONLY);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	i = 0;
	while (line && line[0] != '\0')
	{
		while (line && line[0] != ' ' && line[0] != '1')
			(free(line), line = get_next_line(game->fd_map));
		if (!line)
			break ;
		game->map.grid[i] = ft_calloc(sizeof(char *), ft_strlen(line) + 1);
		if (i == game->map.height - 1)
			ft_strlcpy(game->map.grid[i++], line, ft_strlen(line) + 1);
		else
			ft_strlcpy(game->map.grid[i++], line, ft_strlen(line));
		free(line);
		line = get_next_line(game->fd_map);
	}
	close(game->fd_map);
}

int	count_lines(t_game *game)
{
	char	*line;
	int		height;

	game->fd_map = open(game->map_path, O_RDWR);
	if (game->fd_map == -1)
		read_error(game);
	line = get_next_line(game->fd_map);
	height = 0;
	while (line)
	{
		while (line && line[0] != ' ' && line[0] != '1')
		{
			free(line);
			line = get_next_line(game->fd_map);
		}
		free(line);
		line = get_next_line(game->fd_map);
		height++;
	}
	close(game->fd_map);
	return (height);
}

void	*validate_player_count(t_game *game)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = -1;
	while (++i < game->map.height)
	{
		j = 0;
		while (game->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S' ||
				game->map.grid[i][j] == 'W' || game->map.grid[i][j] == 'E')
			{
				(player_count++);
				game->map.player_start = game->map.grid[i][j];
				game->map.player_pos = vector_create(j, i);
			}
			j++;
		}
	}
	if (player_count != 1)
		return (printf("Error\nMap must contain one player.\n"),
				cleanup_game(game), exit(1), NULL);
	return (NULL);
}
