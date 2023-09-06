/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_child_signals.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 11:29:24 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/06 16:30:15 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_handle_child_signals(void)
{
	struct sigaction	act;

	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	if (sigemptyset(&(act.sa_mask)) == -1)
		return (ft_perror("act sigemptyset"), 0);
	if (sigaction(SIGINT, &act, NULL) == -1)
		return (ft_perror("SIGINT sigaction"), 0);
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		return (ft_perror("SIGQUIT sigaction"), 0);
	return (1);
}
