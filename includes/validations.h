/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:32:52 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:55:43 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#define ERROR_USAGE RED "Game usage: " GREEN "./cub3d maps/valid_map.cub\n"
#define ERROR_MAPTYPE B_RED "Invalid map extension\n"

typedef struct s_game t_game;// <- struct in cub3d.h
typedef struct s_map t_map;// <- struct in cub3d.h

//parse/map_checks.c
int		check_wall(t_map map);
int		map_caracter(t_game *game);

//parse/parse.c
void	game_parse(t_game *game, char **argv);
int	open_map_file(const char *file);

//parse/validate_map.c
bool	validate_map_extension(int argc, char **argv);
bool	valid_extension(char *argv);

#endif
