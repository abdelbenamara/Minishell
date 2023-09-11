/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 17:11:43 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 20:10:28 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum = 0;

static int	ft_event_hook(void)
{
	return (EXIT_SUCCESS);
}

static void	ft_sigint(int signum)
{
	g_signum = signum;
	while (waitpid(-1, NULL, WUNTRACED) != -1)
		;
	if (errno != ECHILD)
		ft_perror("wait");
	rl_replace_line("", 1);
	rl_done = 1;
}

uint8_t	ft_signals(void)
{
	struct sigaction	usr_act;
	struct sigaction	ign_act;

	rl_event_hook = &ft_event_hook;
	usr_act.sa_handler = &ft_sigint;
	usr_act.sa_flags = 0;
	if (sigemptyset(&(usr_act.sa_mask)) == -1)
		return (ft_perror("usr_act sigemptyset"), 0);
	if (sigaction(SIGINT, &usr_act, NULL) == -1)
		return (ft_perror("SIGINT sigaction"), 0);
	ign_act.sa_handler = SIG_IGN;
	ign_act.sa_flags = 0;
	if (sigemptyset(&(ign_act.sa_mask)) == -1)
		return (ft_perror("ign_act sigemptyset"), 0);
	if (sigaction(SIGQUIT, &ign_act, NULL) == -1)
		return (ft_perror("SIGQUIT sigaction"), 0);
	return (1);
}
