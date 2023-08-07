/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/07 04:50:23 by abenamar         ###   ########.fr       */
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
	ft_lst_pop(cmds, &free);
	if (here_doc)
		ft_here_document((*cmds)->content, writefd);
	else
	{
		if (!ft_redirect_input((*cmds)->content, writefd))
		{
			while (*cmds && ft_strncmp((*cmds)->content, "|", 2))
				ft_lst_pop(cmds, &free);
			return (ft_lst_pop(cmds, &free), 0);
		}
	}
	return (ft_lst_pop(cmds, &free), 1);
}

static void	ft_child_do(t_list **cmds, t_list **env, int *writefd, int *readfd)
{
	static void	(*lc)(t_list **, void (*)(void *)) = &ft_lstclear;
	int			code;
	char		*cmd;

	if (close(writefd[1]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	if (dup2(writefd[0], STDIN_FILENO) == -1)
		(perror("dup2"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	if (close(readfd[0]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	if (dup2(readfd[1], STDOUT_FILENO) == -1)
		(perror("dup2"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	code = EXIT_SUCCESS;
	cmd = NULL;
	if (*cmds)
		cmd = (*cmds)->content;
	if (cmd && ft_strncmp(cmd, "|", 2)
		&& ft_strncmp(cmd, ">>", 3) && ft_strncmp(cmd, ">", 2))
		code = ft_execute_command(cmd, env);
	if (close(writefd[0]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	if (close(readfd[1]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	(lc(cmds, &free), lc(env, &free), exit(code));
}

static uint8_t	ft_handle_output(t_list **cmds, int *readfd, int fd)
{
	char	*str;
	int		openflag;

	if (!(*cmds) || !ft_strncmp((*cmds)->content, "|", 2))
	{
		ft_lst_pop(cmds, &free);
		str = get_next_line(readfd[0]);
		while (str)
			(ft_putstr_fd(str, fd), free(str), str = get_next_line(readfd[0]));
		if (close(readfd[0]) == -1)
			return (perror("close"), 0);
		return (1);
	}
	if (!ft_strncmp((*cmds)->content, ">>", 3))
		openflag = O_APPEND;
	else if (!ft_strncmp((*cmds)->content, ">", 2))
		openflag = O_TRUNC;
	else
		return (1);
	ft_lst_pop(cmds, &free);
	if (!ft_redirect_output((*cmds)->content, openflag, readfd))
		return (ft_lst_pop(cmds, &free), 0);
	ft_lst_pop(cmds, &free);
	return (1);
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
	if (ft_strncmp(str, "|", 2)
		&& ft_strncmp(str, ">>", 3) && ft_strncmp(str, ">", 2))
		ft_lst_pop(cmds, &free);
	if (*cmds && !ft_strncmp((*cmds)->content, "|", 2))
	{
		if (pipe(writefd) == -1)
			return (perror("pipe"), 0);
		return (ft_handle_output(cmds, readfd, writefd[1]));
	}
	return (ft_handle_output(cmds, readfd, STDOUT_FILENO));
}

int	ft_pipe_command(t_list **cmds, t_list **env, int *writefd, int *readfd)
{
	pid_t	cpid;
	int		wstatus;

	if (!ft_handle_input(cmds, writefd))
		return (-1);
	if (pipe(readfd) == -1)
		return (perror("pipe"), -1);
	cpid = fork();
	if (cpid == -1)
		return (perror("fork"), -1);
	if (!cpid)
		ft_child_do(cmds, env, writefd, readfd);
	else
	{
		if (close(writefd[1]) == -1)
			return (perror("close"), -1);
		if (waitpid(cpid, &wstatus, 0) == -1)
			return (perror((*cmds)->content), -1);
		if (!ft_parent_do(cmds, writefd, readfd))
			return (-1);
		if (*cmds)
			wstatus = ft_pipe_command(cmds, env, writefd, readfd);
	}
	return (wstatus);
}
