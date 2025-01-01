/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:32:52 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/01 18:19:28 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef VALIDATIONS_H
# define VALIDATIONS_H

# include <colors.h>

# define ERROR_USAGE RED "Game usage: " GREEN "./cub3d maps/valid_map.cub\n"
# define ERROR_MAPTYPE B_RED "Invalid map extension\n"

typedef struct s_game	t_game;
typedef struct s_map	t_map;

// parse/map_checks.c
int						check_wall(t_map map);
int						map_caracter(t_game *game);

// parse/parse.c
void					game_parse(t_game *game, char *map_path);
void					parse_map(t_game *game, char *map_path);
void					set_map(t_game *game, int map_fd);
int						open_map_file(const char *file);

// parse/validate_map.c
bool					validate_map_extension(int argc, char **argv);
bool					valid_extension(char *argv);

#endif
