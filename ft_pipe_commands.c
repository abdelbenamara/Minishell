/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 20:24:31 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_handle_input(t_list **cmds, int *writefd)
{
	const uint8_t	here_doc = !ft_strncmp((*cmds)->content, "<<", 3);

	if (!here_doc && ft_strncmp((*cmds)->content, "<", 2))
		return (1);
	if (close(writefd[1]) == -1)
		return (perror("close"), 0);
	if (close(writefd[0]) == -1)
		return (perror("close"), 0);
	if (pipe(writefd) == -1)
		return (perror("pipe"), 0);
	if (here_doc)
	{
		ft_lst_pop(cmds, &free);
		ft_here_document((*cmds)->content, writefd);
		ft_lst_pop(cmds, &free);
	}
	else
	{
		ft_lst_pop(cmds, &free);
		ft_redirect_input((*cmds)->content, writefd);
		ft_lst_pop(cmds, &free);
	}
	return (1);
}

static void	ft_child_do(t_list **cmds, char **env, int *writefd, int *readfd)
{
	int		exit_code;

	if (close(writefd[1]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (dup2(writefd[0], STDIN_FILENO) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	if (close(readfd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (dup2(readfd[1], STDOUT_FILENO) == -1)
		(perror("dup2"), exit(EXIT_FAILURE));
	exit_code = EXIT_SUCCESS;
	if (*cmds)
		exit_code = ft_execute_command((*cmds)->content, env);
	if (close(writefd[0]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	if (close(readfd[1]) == -1)
		(perror("close"), exit(EXIT_FAILURE));
	ft_lstclear(cmds, &free);
	exit(exit_code);
}

static void	ft_handle_output(t_list **cmds, int *readfd)
{
	char	*str;

	if (!(*cmds))
	{
		str = get_next_line(readfd[0]);
		while (str)
			(ft_putstr_fd(str, STDOUT_FILENO), \
				free(str), str = get_next_line(readfd[0]));
		if (close(readfd[0]) == -1)
			return (perror("close"));
	}
	else if (!ft_strncmp((*cmds)->content, ">>", 3))
	{
		ft_lst_pop(cmds, &free);
		ft_redirect_output((*cmds)->content, O_APPEND, readfd);
		ft_lst_pop(cmds, &free);
	}
	else if (!ft_strncmp((*cmds)->content, ">", 2))
	{
		ft_lst_pop(cmds, &free);
		ft_redirect_output((*cmds)->content, O_TRUNC, readfd);
		ft_lst_pop(cmds, &free);
	}
}

static uint8_t	ft_parent_do(t_list **cmds, int *writefd, int *readfd)
{
	char	*str;

	if (close(writefd[0]) == -1)
		return (perror("close"), 0);
	if (close(readfd[1]) == -1)
		return (perror("close"), 0);
	if (!(*cmds))
		return (1);
	str = (*cmds)->content;
	if (ft_strncmp(str, ">>", 3) && ft_strncmp(str, ">", 2))
		ft_lst_pop(cmds, &free);
	ft_handle_output(cmds, readfd);
	if (*cmds)
	{
		if (pipe(writefd) == -1)
			return (perror("pipe"), 0);
		str = get_next_line(readfd[0]);
		while (str)
			(ft_putstr_fd(str, writefd[1]), \
				free(str), str = get_next_line(readfd[0]));
		if (close(readfd[0]) == -1)
			return (perror("close"), 0);
	}
	return (1);
}

int	ft_pipe_commands(t_list **cmds, char **env, int *writefd, int *readfd)
{
	pid_t	cpid;
	int		wstatus;

	if (!ft_handle_input(cmds, writefd))
		(ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
	if (pipe(readfd) == -1)
		(perror("pipe"), ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
	cpid = fork();
	if (cpid == -1)
		(perror("fork"), ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
	if (!cpid)
		ft_child_do(cmds, env, writefd, readfd);
	else
	{
		if (close(writefd[1]) == -1)
			(perror("close"), ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
		if (waitpid(cpid, &wstatus, 0) == -1)
			(perror((*cmds)->content), \
				ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
		if (!ft_parent_do(cmds, writefd, readfd))
			(ft_lstclear(cmds, &free), exit(EXIT_FAILURE));
		if (*cmds)
			wstatus = ft_pipe_commands(cmds, env, writefd, readfd);
	}
	return (wstatus);
}
