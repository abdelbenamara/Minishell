/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 14:33:31 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 19:58:50 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_redirect_output(char *filename, int openflag, int *readfd)
{
	int		fd;
	char	*str;

	fd = open(filename, O_WRONLY | O_CREAT | openflag, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
		return (perror(filename), 0);
	str = get_next_line(readfd[0]);
	while (str)
		(ft_putstr_fd(str, fd), free(str), str = get_next_line(readfd[0]));
	if (close(readfd[0]) == -1)
		return (perror("close"), 0);
	if (close(fd) == -1)
		return (perror(filename), 0);
	return (1);
}
