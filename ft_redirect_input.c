/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:04:18 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 17:59:12 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_redirect_input(char *filename, int *writefd)
{
	int		fd;
	char	*str;
	size_t	dlines;

	fd = open(filename, O_RDONLY | O_NONBLOCK);
	if (fd == -1)
		return (perror(filename), 0);
	str = get_next_line(fd);
	dlines = 0;
	if (!ft_strncmp(filename, "/dev/", 5))
		while (++dlines <= DLINES_MAX && str)
			(ft_putstr_fd(str, writefd[1]), free(str), str = get_next_line(fd));
	if (str && dlines)
		(free(str), str = NULL);
	while (str)
		(ft_putstr_fd(str, writefd[1]), free(str), str = get_next_line(fd));
	if (close(fd) == -1)
		return (perror(filename), 0);
	return (1);
}
