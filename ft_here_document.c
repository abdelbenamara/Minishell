/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_document.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:05:46 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 21:35:06 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_is_eof(char *str, char *limiter, size_t len)
{
	if (!ft_strncmp(str, limiter, len) && ((isatty(STDIN_FILENO) && !(str[len]))
			|| (!isatty(STDIN_FILENO) && str[len] == '\n')))
		return (1);
	return (0);
}

uint8_t	ft_here_document(char *limiter, int *writefd)
{
	const char		*warn = "here-document delimited by end-of-file";
	const size_t	len = ft_strlen(limiter);
	char			*str;

	if (isatty(STDIN_FILENO))
		str = readline("> ");
	else
		str = get_next_line(STDIN_FILENO);
	while (str && !ft_is_eof(str, limiter, len))
	{
		if (g_signum == SIGINT)
			return (0);
		ft_putstr_fd(str, writefd[1]);
		free(str);
		if (isatty(STDIN_FILENO))
			(ft_putstr_fd("\n", writefd[1]), str = readline("> "));
		else
			str = get_next_line(STDIN_FILENO);
	}
	if (!str)
		ft_printf("minishell: warning: %s (wanted `%s')\n", warn, limiter);
	free(str);
	return (1);
}
