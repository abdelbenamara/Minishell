/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prc_new.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 22:26:33 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/10 22:27:33 by abenamar         ###   ########.fr       */
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
	proc->readfd = NULL;
	proc->writefd = NULL;
	return (proc);
}
