/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:51:16 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/11 19:36:24 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_input(t_list **cmds, int *writefd)
{
	const uint8_t	here_doc = !ft_strncmp((*cmds)->content, "<<", 3);

	if (!here_doc && ft_strncmp((*cmds)->content, "<", 2))
		return (EXIT_SUCCESS);
	if (close(writefd[1]) == -1)
		return (ft_perror("close"), -1);
	if (close(writefd[0]) == -1)
		return (ft_perror("close"), -1);
	if (pipe(writefd) == -1)
		return (ft_perror("pipe"), -1);
	ft_lst_pop(cmds, &free);
	if (!(*cmds) || !ft_strncmp((*cmds)->content, "|", 2))
		return (ft_pstderr("syntax error near unexpected token `newline'"), -2);
	if (here_doc)
	{
		if (!ft_here_document((*cmds)->content, writefd))
			return (ft_lstclear(cmds, &free), -1);
	}
	else if (!ft_redirect_input((*cmds)->content, writefd))
	{
		while (*cmds && ft_strncmp((*cmds)->content, "|", 2))
			ft_lst_pop(cmds, &free);
		return (ft_lst_pop(cmds, &free), -1);
	}
	return (ft_lst_pop(cmds, &free), EXIT_SUCCESS);
}
