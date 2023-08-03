/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 14:33:31 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/02 20:27:09 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_redirect_output(char *filename, int openflag, int *readfd)
{
	int		fd;
	char	*str;

	fd = open(filename, O_WRONLY | O_CREAT | openflag, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		return (perror(filename));
	str = get_next_line(readfd[0]);
	while (str)
		(ft_putstr_fd(str, fd), free(str), str = get_next_line(readfd[0]));
	if (close(readfd[0]) == -1)
		return (perror("close"));
	if (close(fd) == -1)
		return (perror(filename));
}
