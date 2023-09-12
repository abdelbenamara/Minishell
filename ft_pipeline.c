/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/12 09:17:32 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_child_setup(t_list *lst, int *readfd, int *writefd)
{
	if (readfd[0] && !(ft_tkn_count(lst, "<<") + ft_tkn_count(lst, "<")))
	{
		if (close(readfd[1]) == -1)
			return (ft_perror("close"), 0);
		if (dup2(readfd[0], STDIN_FILENO) == -1)
			return (ft_perror("dup2"), 0);
		if (close(readfd[0]) == -1)
			return (ft_perror("close"), 0);
	}
	if (writefd[1])
	{
		if (close(writefd[0]) == -1)
			return (ft_perror("close"), 0);
		if (dup2(writefd[1], STDOUT_FILENO) == -1)
			return (ft_perror("dup2"), 0);
		if (close(writefd[1]) == -1)
			return (ft_perror("close"), 0);
	}
	return (1);
}

static uint8_t	ft_fork(t_list **prcs, t_list **tkns, t_list **env)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == -1)
		return (ft_perror("fork"), ft_lstclear(tkns, &free), 0);
	if (!cpid)
	{
		rl_clear_history();
		if (!ft_child_setup(*tkns, \
				((t_proc *)(*prcs)->content)->readfd, \
				((t_proc *)(*prcs)->content)->writefd))
			(ft_lstclear(prcs, &ft_prc_del), ft_lstclear(tkns, &free), \
				ft_lstclear(env, &free), exit(EXIT_FAILURE));
		ft_lstclear(prcs, &ft_prc_del);
		return (ft_child_execute(tkns, env), 1);
	}
	((t_proc *)(*prcs)->content)->pid = cpid;
	if (((t_proc *)(*prcs)->content)->readfd[0])
	{
		if (close(((t_proc *)(*prcs)->content)->readfd[1]) == -1)
			return (ft_perror("close"), 0);
		if (close(((t_proc *)(*prcs)->content)->readfd[0]) == -1)
			return (ft_perror("close"), 0);
	}
	return (1);
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
		return (ft_perror("wait"), EXIT_FAILURE);
	if (code != EXIT_SUCCESS)
		return (code);
	if (WIFSIGNALED(wstatus))
	{
		g_signum = WTERMSIG(wstatus);
		if (g_signum == SIGQUIT)
			ft_printf("Quit");
		ft_printf("\n");
		return (128 + g_signum);
	}
	return (WEXITSTATUS(wstatus));
}

int	ft_pipeline(t_list **tkns, t_list **env)
{
	t_list	*prcs;

	prcs = ft_lstnew(ft_prc_new());
	while (*tkns)
	{
		ft_lstadd_front(&prcs, ft_lstnew(ft_prc_new()));
		((t_proc *) prcs->content)->readfd[0] = \
			((t_proc *) prcs->next->content)->writefd[0];
		((t_proc *) prcs->content)->readfd[1] = \
			((t_proc *) prcs->next->content)->writefd[1];
		if (ft_tkn_count(*tkns, "|"))
		{
			if (pipe(((t_proc *) prcs->content)->writefd) == -1)
				return (ft_perror("pipe"), ft_lstclear(tkns, &free), \
					ft_wait(&prcs, EXIT_FAILURE));
		}
		if (!ft_fork(&prcs, tkns, env))
			return (ft_lstclear(&prcs, &ft_prc_del), EXIT_FAILURE);
		while (*tkns && ft_strncmp((*tkns)->content, "|", 2))
			ft_lst_pop(tkns, &free);
		ft_lst_pop(tkns, &free);
	}
	return (ft_wait(&prcs, EXIT_SUCCESS));
}
