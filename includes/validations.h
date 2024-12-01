/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:32:52 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 17:20:22 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATIONS_H
#define VALIDATIONS_H

typedef struct s_game t_game;// <- struct in cub3d.h
typedef struct s_map t_map;// <- struct in cub3d.h

//parse/map_checks.c
bool	validate_map(t_game *game, char **argv);
int		check_wall(t_map map);
int		map_caracter(t_game *game);

//parse/parse.c
int	open_map_file(const char *file);
int	check_file_ext(const char *file);

//parse/validate_map.c
bool	validate_map(t_game *game, char **argv);
bool	valid_extension(char *argv);

#endif
