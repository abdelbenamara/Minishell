/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:02:17 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 13:44:11 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_lst_remove_last(t_list *lst, void (*del)(void *))
{
	while (lst && lst->next)
	{
		lst = lst->next;
	}
	if (lst->next)
		ft_lstdelone(lst->next, del);
	lst->next = NULL;
}

static uint8_t	ft_child_setup(int *readfd, int *writefd)
{
	if (readfd)
	{
		if (close(readfd[1]) == -1)
			return (ft_perror("close"), 0);
		if (dup2(readfd[0], STDIN_FILENO) == -1)
			return (ft_perror("dup2"), 0);
		if (close(readfd[0]) == -1)
			return (ft_perror("close"), 0);
	}
	if (writefd)
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
	int		*readfd;
	int		*writefd;

	cpid = fork();
	if (cpid == -1)
		return (ft_perror("fork"), ft_lstclear(tkns, &free), 0);
	if (!cpid)
	{
		rl_clear_history();
		readfd = ((t_proc *)(*prcs)->content)->readfd;
		writefd = ((t_proc *)(*prcs)->content)->writefd;
		if (!ft_child_setup(readfd, writefd))
			(ft_child_exit(prcs, tkns, env), exit(EXIT_FAILURE));
		return (ft_child_execute(prcs, tkns, env), 1);
	}
	((t_proc *)(*prcs)->content)->pid = cpid;
	return (1);
}

int	ft_wait(t_list **prcs)
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
	t_list			*prcs;
	int				pipefd[2];

	prcs = ft_lstnew(ft_prc_new());
	while (*tkns)
	{
		ft_lstadd_front(&prcs, ft_lstnew(ft_prc_new()));
		((t_proc *) prcs->content)->readfd = \
			((t_proc *) prcs->next->content)->writefd;
		if (ft_tkn_count(*tkns, "|"))
		{
			if (pipe(pipefd) == -1)
				return (ft_perror("pipe"), ft_lstclear(tkns, &free), \
					ft_lstclear(&prcs, &ft_prc_del), EXIT_FAILURE);
			((t_proc *) prcs->content)->writefd = pipefd;
		}
		while (prcs->next->next)
			ft_lst_remove_last(prcs, &ft_prc_del);
		if (!ft_fork(&prcs, tkns, env))
			return (ft_lstclear(&prcs, &ft_prc_del), EXIT_FAILURE);
		while (*tkns && ft_strncmp((*tkns)->content, "|", 2))
			ft_lst_pop(tkns, &free);
		ft_lst_pop(tkns, &free);
	}
	return (ft_wait(&prcs));
}
