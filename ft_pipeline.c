/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 16:50:52 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_pipe_setup(t_proc *prc, t_list *lst)
{
	if (prc->readfd[0])
	{
		if (close(prc->readfd[1]) == -1)
			return (ft_perror("close: "), 0);
		if (!ft_tkn_count(lst, "<<", 0) && !ft_tkn_count(lst, "<", 0)
			&& dup2(prc->readfd[0], STDIN_FILENO) == -1)
			return (ft_perror("dup2: "), 0);
		if (close(prc->readfd[0]) == -1)
			return (ft_perror("close: "), 0);
	}
	if (prc->writefd[1])
	{
		if (close(prc->writefd[0]) == -1)
			return (ft_perror("close: "), 0);
		if (!ft_tkn_count(lst, ">>", 0) && !ft_tkn_count(lst, ">", 0)
			&& dup2(prc->writefd[1], STDOUT_FILENO) == -1)
			return (ft_perror("dup2: "), 0);
		if (close(prc->writefd[1]) == -1)
			return (ft_perror("close: "), 0);
	}
	return (1);
}

static uint8_t	ft_fork(t_list **prcs, t_list **tkns, t_list **env)
{
	pid_t	cpid;

	if (!(*prcs) || !((*prcs)->content))
		return (0);
	cpid = fork();
	if (cpid == -1)
		return (ft_perror("fork: "), ft_lstclear(tkns, &free), 0);
	if (!cpid)
	{
		rl_clear_history();
		if (!ft_pipe_setup((*prcs)->content, *tkns))
			(ft_lstclear(prcs, &ft_prc_del), ft_lstclear(tkns, &free), \
				ft_lstclear(env, &free), exit(EXIT_FAILURE));
		ft_lstclear(prcs, &ft_prc_del);
		return (ft_execute(tkns, env), 1);
	}
	if (((t_proc *)(*prcs)->content)->readfd[0])
	{
		if (close(((t_proc *)(*prcs)->content)->readfd[1]) == -1)
			return (ft_perror("close: "), 0);
		if (close(((t_proc *)(*prcs)->content)->readfd[0]) == -1)
			return (ft_perror("close: "), 0);
	}
	return (((t_proc *)(*prcs)->content)->pid = cpid, 1);
}

int	ft_wait(t_list **prcs, int code)
{
	pid_t	cpid;
	int		wstatus;

	cpid = ((t_proc *)(*prcs)->content)->pid;
	ft_lstclear(prcs, &ft_prc_del);
	if (waitpid(cpid, &wstatus, WUNTRACED) == -1)
		return (EXIT_FAILURE);
	while (waitpid(-1, NULL, WUNTRACED) != -1)
		;
	if (errno != ECHILD)
		return (ft_perror("wait: "), EXIT_FAILURE);
	if (code != EXIT_SUCCESS)
		return (code);
	if (WIFSIGNALED(wstatus))
	{
		g_signum = WTERMSIG(wstatus);
		if (g_signum == SIGQUIT)
			printf("Quit");
		printf("\n");
		return (128 + g_signum);
	}
	return (WEXITSTATUS(wstatus));
}

static uint8_t	ft_next_pipe(t_list **prcs, t_list **tkns)
{
	ft_lstadd_front(prcs, ft_lstnew(ft_prc_new()));
	((t_proc *)(*prcs)->content)->readfd[0] = \
		((t_proc *)(*prcs)->next->content)->writefd[0];
	((t_proc *)(*prcs)->content)->readfd[1] = \
		((t_proc *)(*prcs)->next->content)->writefd[1];
	if (ft_tkn_count(*tkns, "|", 1))
	{
		if (pipe(((t_proc *)(*prcs)->content)->writefd) == -1)
			return (ft_perror("pipe: "), 0);
	}
	return (1);
}

int	ft_pipeline(t_list **tkns, t_list **env)
{
	t_list	*prcs;

	prcs = ft_lstnew(ft_prc_new());
	if (!ft_tkn_count(*tkns, "|", 1))
	{
		if (!ft_fork(&prcs, tkns, env))
			return (ft_lstclear(tkns, &free), ft_wait(&prcs, EXIT_FAILURE));
		return (ft_wait(&prcs, EXIT_SUCCESS));
	}
	while (*tkns)
	{
		if (!ft_next_pipe(&prcs, tkns))
			return (ft_lstclear(tkns, &free), ft_wait(&prcs, EXIT_FAILURE));
		if (!ft_fork(&prcs, tkns, env))
			return (ft_lstclear(tkns, &free), ft_wait(&prcs, EXIT_FAILURE));
		while (*tkns && ft_strncmp((*tkns)->content, "|", 2))
		{
			if (!ft_strncmp((*tkns)->content, "<<", 3))
				ft_close(ft_atoi((*tkns)->next->content));
			ft_lst_pop(tkns, &free);
		}
		ft_lst_pop(tkns, &free);
	}
	return (ft_wait(&prcs, EXIT_SUCCESS));
}
