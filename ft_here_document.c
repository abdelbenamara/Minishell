/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_document.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:05:46 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 03:54:09 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_write_line(uint8_t expand, char *str, t_list **env, int fd)
{
	char	*exp;

	if (expand)
	{
		exp = ft_expand(str, env, 0, 1);
		ft_putstr_fd(exp, fd);
		free(exp);
	}
	else
		ft_putstr_fd(str, fd);
}

static uint8_t	ft_read_lines(uint8_t expand, t_list **env, char *eof, int fd)
{
	const size_t	len = ft_strlen(eof);
	char			*str;

	if (isatty(STDIN_FILENO))
		str = readline("> ");
	else
		str = get_next_line(STDIN_FILENO);
	while (str && (ft_strncmp(str, eof, len)
			|| ((isatty(STDIN_FILENO) && str[len])
				|| (!isatty(STDIN_FILENO) && str[len] != '\n'))))
	{
		ft_write_line(expand, str, env, fd);
		free(str);
		if (g_signum == SIGINT)
			return (free(eof), 0);
		if (isatty(STDIN_FILENO))
			(ft_putstr_fd("\n", fd), str = readline("> "));
		else
			str = get_next_line(STDIN_FILENO);
	}
	if (!str)
		(ft_printf("minishell: warning: here-document"), \
			ft_printf(" delimited by end-of-file (wanted `%s')\n", eof));
	free(str);
	return (free(eof), 1);
}

uint8_t	ft_read_document(char *limiter, t_list **env, int fd)
{
	size_t	i;
	uint8_t	expand;
	char	*eof;

	if (!limiter)
		return (0);
	i = 0;
	expand = 1;
	while (limiter[i])
	{
		if (ft_is_quoted(limiter + i))
		{
			expand = 0;
			break ;
		}
		++i;
	}
	eof = ft_command_setup(limiter, '\0', '\0', 0);
	if (!eof)
		return (0);
	return (ft_read_lines(expand, env, eof, fd));
}

uint8_t	ft_here_document(char **limiter, t_list **env, size_t last)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (ft_perror2("here_doc", "pipe"), 0);
	if (!ft_read_document(*limiter, env, pipefd[1]))
	{
		if (close(pipefd[1]) == -1)
			ft_perror2("here_doc", "close");
		if (close(pipefd[0]) == -1)
			ft_perror2("here_doc", "close");
		return (0);
	}
	if (close(pipefd[1]) == -1)
		return (ft_perror2("here_doc", "close"), 0);
	if (last)
	{
		free(*limiter);
		*limiter = ft_itoa(pipefd[0]);
	}
	else if (close(pipefd[0]) == -1)
		return (ft_perror2("here_doc", "close"), 0);
	return (1);
}
