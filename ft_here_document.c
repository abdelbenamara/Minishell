/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_document.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:05:46 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 16:47:42 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_here_document(char *limiter, int *writefd)
{
	const char		*warn = "here-document delimited by end-of-file";
	const size_t	len = ft_strlen(limiter) + 1;
	char			*str;

	str = readline("> ");
	while (str && ft_strncmp(str, limiter, len))
	{
		ft_putstr_fd(str, writefd[1]);
		ft_putstr_fd("\n", writefd[1]);
		free(str);
		str = readline("> ");
	}
	if (!str)
		ft_printf("warning: %s (wanted `%s')\n", warn, limiter);
	free(str);
}
