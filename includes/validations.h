/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:32:52 by marsoare          #+#    #+#             */
/*   Updated: 2024/12/01 16:48:33 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATIONS_H
#define VALIDATIONS_H

//parse/map_checks.c
bool	validate_map(t_game *game, char **argv);
int		check_wall(t_map map);
int		map_caracter(t_game *game);

#endif
