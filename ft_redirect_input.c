/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:04:18 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/07 18:17:37 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_redirect_input(char *filename, int *writefd)
{
	const uint8_t	device = !ft_strncmp(filename, "/dev/", 5);
	int				fd;
	char			*str;
	size_t			dlines;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror(filename), 0);
	str = get_next_line(fd);
	dlines = 0;
	while (dlines < DLINES_MAX && str)
	{
		ft_putstr_fd(str, writefd[1]);
		free(str);
		str = get_next_line(fd);
		if (device && str && str[ft_strlen(str) - 1] == '\n')
			++dlines;
	}
	if (close(fd) == -1)
		return (perror(filename), free(str), 0);
	if (g_signum == SIGINT)
		return (ft_printf("\n"), free(str), 0);
	return (free(str), 1);
}
