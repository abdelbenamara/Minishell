/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 14:39:49 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_input(t_list **cmds, int *writefd)
{
	const uint8_t	here_doc = !ft_strncmp((*cmds)->content, "<<", 3);

	if (!here_doc && ft_strncmp((*cmds)->content, "<", 2))
		return (1);
	if (close(writefd[1]) == -1)
		return (perror("close"), -1);
	if (close(writefd[0]) == -1)
		return (perror("close"), -1);
	if (pipe(writefd) == -1)
		return (perror("pipe"), -1);
	ft_lst_pop(cmds, &free);
	if (!(*cmds) || !ft_strncmp((*cmds)->content, "|", 2))
		return (ft_printf("syntax error near unexpected token `newline'\n"), -2);
	if (here_doc)
	{
		if (!ft_here_document((*cmds)->content, writefd))
			return (ft_lstclear(cmds, &free), -1);
	}
	else if (!ft_redirect_input((*cmds)->content, writefd))
	{
		while (*cmds && ft_strncmp((*cmds)->content, "|", 2))
			ft_lst_pop(cmds, &free);
		return (ft_lst_pop(cmds, &free), -1);
	}
	return (ft_lst_pop(cmds, &free), EXIT_SUCCESS);
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
	code = ft_execute_builtin(cmd, env);
	if (code == 127)
		code = ft_execute_command(cmd, env);
	if (close(writefd[0]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	if (close(readfd[1]) == -1)
		(perror("close"), lc(cmds, &free), lc(env, &free), exit(EXIT_FAILURE));
	(lc(cmds, &free), lc(env, &free), exit(code));
}

static int	ft_handle_output(t_list **cmds, int *readfd, int fd, int wstatus)
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
			return (perror("close"), -1);
		return (wstatus);
	}
	if (!ft_strncmp((*cmds)->content, ">>", 3))
		openflag = O_APPEND;
	else if (!ft_strncmp((*cmds)->content, ">", 2))
		openflag = O_TRUNC;
	else
		return (wstatus);
	ft_lst_pop(cmds, &free);
	if (!(*cmds) || !ft_strncmp((*cmds)->content, "|", 2))
		return (ft_printf("syntax error near unexpected token `newline'\n"), -2);
	if (!ft_redirect_output((*cmds)->content, openflag, readfd))
		return (ft_lst_pop(cmds, &free), -1);
	return (ft_lst_pop(cmds, &free), wstatus);
}

static int	ft_parent_do(t_list **cmds, int *writefd, int *readfd, int wstatus)
{
	char	*str;

	if (close(writefd[0]) == -1)
		return (perror("close"), -1);
	if (close(readfd[1]) == -1)
		return (perror("close"), -1);
	if (!(*cmds))
		return (wstatus);
	str = (*cmds)->content;
	if (ft_strncmp(str, "|", 2)
		&& ft_strncmp(str, ">>", 3) && ft_strncmp(str, ">", 2))
		ft_lst_pop(cmds, &free);
	if (wstatus < 0)
		return (wstatus);
	if (*cmds && !ft_strncmp((*cmds)->content, "|", 2))
	{
		if (pipe(writefd) == -1)
			return (perror("pipe"), -1);
		return (ft_handle_output(cmds, readfd, writefd[1], wstatus));
	}
	return (ft_handle_output(cmds, readfd, STDOUT_FILENO, wstatus));
}

int	ft_pipe_command(t_list **cmds, t_list **env, int *writefd, int *readfd)
{
	pid_t	cpid;
	int		wstatus;

	if (pipe(readfd) == -1)
		return (perror("pipe"), -1);
	wstatus = ft_handle_input(cmds, writefd);
	wstatus = ft_handle_exit(cmds, env, wstatus);
	if (wstatus < 0)
		return (wstatus);
	cpid = fork();
	if (cpid == -1)
		return (perror("fork"), -1);
	if (!cpid)
		return (ft_child_do(cmds, env, writefd, readfd), 0);
	if (close(writefd[1]) == -1)
		return (perror("close"), -1);
	if (waitpid(cpid, &wstatus, 0) == -1)
		return (perror((*cmds)->content), -1);
	wstatus = ft_pipe_builtin(cmds, env, wstatus);
	wstatus = ft_parent_do(cmds, writefd, readfd, wstatus);
	if (wstatus < 0)
		return (wstatus);
	if (*cmds)
		wstatus = ft_pipe_command(cmds, env, writefd, readfd);
	return (wstatus);
}
