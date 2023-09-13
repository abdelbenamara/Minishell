/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 00:15:31 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 04:09:44 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_redirection_type(char *str)
{
	if (!ft_strncmp(str, "<<", 3))
		return (1);
	else if (!ft_strncmp(str, "<", 2))
		return (2);
	else if (!ft_strncmp(str, ">>", 2))
		return (3);
	else if (!ft_strncmp(str, ">", 2))
		return (4);
	return (0);
}

static uint8_t	ft_handle_here_document(char *str, size_t last)
{
	if (last)
	{
		if (dup2(ft_atoi(str), STDIN_FILENO) == -1)
			return (ft_perror("dup2"), 0);
		if (close(ft_atoi(str)) == -1)
			return (ft_perror("close"), 0);
	}
	return (1);
}

uint8_t	ft_redirect(t_list **tkns, t_list **env)
{
	uint8_t	type;

	if (!(*tkns) || !ft_is_redirection((*tkns)->content))
		return (1);
	type = ft_redirection_type((*tkns)->content);
	ft_lst_pop(tkns, &free);
	if (type == 1 && !ft_handle_here_document((*tkns)->content, \
			!(ft_tkn_count(*tkns, "<<", 0) + ft_tkn_count(*tkns, "<", 0))))
		return (0);
	if (type == 2 && !ft_redirect_input((*tkns)->content, env, \
			!(ft_tkn_count(*tkns, "<<", 0) + ft_tkn_count(*tkns, "<", 0))))
		return (0);
	if (type == 3 && !ft_redirect_output((*tkns)->content, O_APPEND, env, \
			!(ft_tkn_count(*tkns, ">>", 0) + ft_tkn_count(*tkns, ">", 0))))
		return (0);
	if (type == 4 && !ft_redirect_output((*tkns)->content, O_TRUNC, env, \
			!(ft_tkn_count(*tkns, ">>", 0) + ft_tkn_count(*tkns, ">", 0))))
		return (0);
	ft_lst_pop(tkns, &free);
	return (ft_redirect(tkns, env));
}
