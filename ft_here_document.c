/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_document.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:05:46 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 18:28:32 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_here_document(char *limiter, int *writefd)
{
	const size_t	len = ft_strlen(limiter) + 2;
	char			*eof;
	char			*str;

	eof = ft_strjoin(limiter, "\n");
	if (!eof)
		return ;
	str = get_next_line(STDIN_FILENO);
	while (ft_strncmp(str, eof, len))
	{
		ft_putstr_fd(str, writefd[1]);
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	free(eof);
	free(str);
}
