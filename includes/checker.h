/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:40:44 by marsoare          #+#    #+#             */
/*   Updated: 2025/01/12 14:12:25 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_H
# define CHECKER_H

//checker.c
bool	checker(t_game *game, char *path);
//flood_fill
bool	is_map_valid(t_game *game);

#endif
