/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_signals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:11:43 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 17:43:34 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum = 0;

static int	ft_event_hook(void)
{
	return (EXIT_SUCCESS);
}

static void	ft_handle_sigint(int signum)
{
	g_signum = signum;
	if (signum == SIGINT)
		rl_done = 1;
}

void	ft_handle_signals(void)
{
	struct sigaction	act;
	struct sigaction	no_act;

	rl_event_hook = &ft_event_hook;
	act.sa_handler = &ft_handle_sigint;
	act.sa_flags = 0;
	if (sigemptyset(&(act.sa_mask)) == -1)
		perror("act sigemptyset");
	if (sigaction(SIGINT, &act, NULL) == -1)
		perror("SIGINT sigaction");
	no_act.sa_handler = SIG_IGN;
	no_act.sa_flags = 0;
	if (sigemptyset(&(no_act.sa_mask)) == -1)
		perror("no_act sigemptyset");
	if (sigaction(SIGQUIT, &no_act, NULL) == -1)
		perror("SIGQUIT sigaction");
}
