/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/08 00:06:32 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_close_fds(int writefd0, int readfd1, int wstatus)
{
	if (wstatus < 0)
		return (wstatus);
	if (close(writefd0) == -1)
		return (ft_perror("close"), -1);
	if (close(readfd1) == -1)
		return (ft_perror("close"), -1);
	return (wstatus);
}

static void	ft_child(t_list **cmds, t_list **env, int *writefd, int *readfd)
{
	static void	(*f)(t_list **, void (*)(void *)) = &ft_lstclear;
	int			code;
	char		*cmd;

	rl_clear_history();
	if (!ft_handle_child_signals())
		(f(cmds, &free), f(env, &free), exit(EXIT_FAILURE));
	if (dup2(writefd[0], STDIN_FILENO) == -1)
		(ft_perror("dup2"), f(cmds, &free), f(env, &free), exit(EXIT_FAILURE));
	if (dup2(readfd[1], STDOUT_FILENO) == -1)
		(ft_perror("dup2"), f(cmds, &free), f(env, &free), exit(EXIT_FAILURE));
	code = EXIT_SUCCESS;
	cmd = NULL;
	if (*cmds)
		cmd = (*cmds)->content;
	code = ft_execute_builtin(cmd, env);
	if (code == 127 && g_signum != SIGTERM)
		code = ft_execute_command(cmd, env);
	if (ft_close_fds(writefd[0], readfd[1], code) == -1)
		(f(cmds, &free), f(env, &free), exit(EXIT_FAILURE));
	if (close(readfd[0]) == -1)
		(ft_perror("close"), f(cmds, &free), f(env, &free), exit(EXIT_FAILURE));
	(f(cmds, &free), f(env, &free), exit(code));
}

static int	ft_redirect(t_list **cmds, int *writefd, int *readfd, int wstatus)
{
	int	fd;

	if (wstatus < 0)
		return (wstatus);
	if (!(*cmds))
	{
		if (close(readfd[0]) == -1)
			return (ft_perror("close"), -1);
		return (wstatus);
	}
	if (!ft_is_redirection((*cmds)->content))
		ft_lst_pop(cmds, &free);
	fd = STDOUT_FILENO;
	if (*cmds && !ft_strncmp((*cmds)->content, "|", 2))
	{
		if (pipe(writefd) == -1)
			return (ft_perror("pipe"), -1);
		fd = writefd[1];
	}
	wstatus = ft_handle_output(cmds, readfd, fd, wstatus);
	if (close(readfd[0]) == -1)
		return (ft_perror("close"), -1);
	if (*cmds && !ft_strncmp((*cmds)->content, "|", 2) && pipe(writefd) == -1)
		return (ft_perror("pipe"), -1);
	return (wstatus);
}

int	ft_handle_pipe(t_list **cmds, t_list **env, int *writefd, int *readfd)
{
	pid_t	cpid;
	int		wstatus;

	if (pipe(readfd) == -1)
		return (ft_perror("pipe"), -1);
	wstatus = ft_handle_input(cmds, writefd);
	if (wstatus < 0)
		return (wstatus);
	if (close(writefd[1]) == -1)
		return (ft_perror("close"), -1);
	cpid = fork();
	if (cpid == -1)
		return (ft_perror("fork"), -1);
	if (!cpid)
		return (ft_child(cmds, env, writefd, readfd), EXIT_SUCCESS);
	if (waitpid(cpid, &wstatus, WUNTRACED) == -1)
		ft_lstclear(cmds, &free);
	wstatus = ft_close_fds(writefd[0], readfd[1], wstatus);
	wstatus = ft_handle_exit(cmds, env, wstatus);
	wstatus = ft_handle_builtin(cmds, env, readfd[0], wstatus);
	wstatus = ft_handle_minishell(cmds, env, wstatus);
	wstatus = ft_redirect(cmds, writefd, readfd, wstatus);
	if (*cmds && wstatus >= 0)
		wstatus = ft_handle_pipe(cmds, env, writefd, readfd);
	return (wstatus);
}
