/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prc_del.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 18:06:25 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 22:13:27 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_prc_del(void *proc)
{
	if (((t_proc *) proc)->readfd[0])
		close(((t_proc *) proc)->readfd[0]);
	if (((t_proc *) proc)->writefd[1])
		close(((t_proc *) proc)->writefd[1]);
	free(proc);
}
