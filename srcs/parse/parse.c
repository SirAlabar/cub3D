/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 21:22:02 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/11/30 21:23:37 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	open_map_file(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Error opening map file %s\n", file);
		return (-1);
	}
	if (!check_file_ext(file))
	{
		ft_printf("Error map extension %s\n", file);
		close(fd);
		return (-1);
	}
	return (fd);
}

int	check_file_ext(const char *file)
{
	size_t		len;
	const char	*exten;

	len = ft_strlen(file);
	if (len < 4)
		return (0);
	exten = file + (len - 4);
	return (!ft_strncmp(".cub", exten, 4));
}
