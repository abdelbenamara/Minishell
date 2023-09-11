/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prc_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:26:33 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 22:29:09 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_proc	*ft_prc_new(void)
{
	t_proc	*proc;

	proc = malloc(sizeof(t_proc));
	if (!proc)
		return (NULL);
	proc->pid = -1;
	proc->readfd[1] = 0;
	proc->readfd[0] = 0;
	proc->writefd[0] = 0;
	proc->writefd[1] = 0;
	return (proc);
}
