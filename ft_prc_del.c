/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prc_del.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 18:06:25 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 00:50:12 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_prc_del(void *prc)
{
	if (((t_proc *) prc)->readfd[0])
		close(((t_proc *) prc)->readfd[0]);
	if (((t_proc *) prc)->writefd[1])
		close(((t_proc *) prc)->writefd[1]);
	free(prc);
}
