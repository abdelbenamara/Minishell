/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_output.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 17:52:33 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 21:31:52 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_output(t_list **cmds, int *readfd, int fd, int wstatus)
{
	char	*str;
	int		openflag;

	if (!(*cmds) || !ft_strncmp((*cmds)->content, "|", 2))
	{
		ft_lst_pop(cmds, &free);
		str = get_next_line(readfd[0]);
		while (str)
			(ft_putstr_fd(str, fd), free(str), str = get_next_line(readfd[0]));
		return (wstatus);
	}
	if (!ft_strncmp((*cmds)->content, ">>", 3))
		openflag = O_APPEND;
	else if (!ft_strncmp((*cmds)->content, ">", 2))
		openflag = O_TRUNC;
	else
		return (wstatus);
	ft_lst_pop(cmds, &free);
	if (!ft_redirect_output((*cmds)->content, openflag, readfd))
		return (ft_lst_pop(cmds, &free), -1);
	ft_lst_pop(cmds, &free);
	if (*cmds && !ft_strncmp((*cmds)->content, "|", 2))
		return (wstatus);
	return (ft_handle_output(cmds, readfd, fd, wstatus));
}
