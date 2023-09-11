/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 18:05:41 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 09:14:39 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_child_exit(t_list **prcs, t_list **tkns, t_list **env)
{
	int		*readfd;
	int		*writefd;

	readfd = ((t_proc *)(*prcs)->content)->readfd;
	writefd = ((t_proc *)(*prcs)->content)->writefd;
	if (readfd)
		close(readfd[1]);
	if (writefd)
		close(writefd[0]);
	ft_lstclear(prcs, &ft_prc_del);
	ft_lstclear(tkns, &free);
	ft_lstclear(env, &free);
}
