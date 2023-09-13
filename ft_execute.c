/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:23:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 16:50:51 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_close_here_documents(t_list *lst)
{
	while (ft_tkn_count(lst, "<<", 1))
	{
		if (!ft_strncmp(lst->content, "<<", 3))
		{
			lst = lst->next;
			close(ft_atoi(lst->content));
		}
		lst = lst->next;
	}
}

static uint8_t	ft_default_signals(void)
{
	struct sigaction	dfl_act;

	dfl_act.sa_handler = SIG_DFL;
	dfl_act.sa_flags = 0;
	if (sigemptyset(&(dfl_act.sa_mask)) == -1)
		return (ft_perror("dfl_act sigemptyset: "), 0);
	if (sigaction(SIGINT, &dfl_act, NULL) == -1)
		return (ft_perror("SIGINT sigaction: "), 0);
	if (sigaction(SIGQUIT, &dfl_act, NULL) == -1)
		return (ft_perror("SIGQUIT sigaction: "), 0);
	return (1);
}

void	ft_execute(t_list **tkns, t_list **env)
{
	int	code;

	if (!ft_redirect(tkns, env))
		(ft_close_here_documents(*tkns), ft_lstclear(tkns, &free), \
			ft_lstclear(env, &free), exit(EXIT_FAILURE));
	ft_close_here_documents(*tkns);
	if (!ft_default_signals())
		(ft_lstclear(tkns, &free), ft_lstclear(env, &free), exit(EXIT_FAILURE));
	if (!(*tkns))
		(ft_lstclear(tkns, &free), ft_lstclear(env, &free), exit(EXIT_SUCCESS));
	code = ft_builtin((*tkns)->content, env, 0);
	if (code == 127 && g_signum != SIGTERM)
		code = ft_execve((*tkns)->content, env);
	ft_lstclear(tkns, &free);
	ft_lstclear(env, &free);
	exit(code);
}
