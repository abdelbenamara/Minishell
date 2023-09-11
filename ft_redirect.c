/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 00:15:31 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 09:31:47 by abenamar         ###   ########.fr       */
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

uint8_t	ft_redirect(t_list **tkns, t_list **env, uint8_t fake)
{
	uint8_t	type;

	if (!(*tkns) || !ft_is_redirection((*tkns)->content))
		return (1);
	type = ft_redirection_type((*tkns)->content);
	ft_lst_pop(tkns, &free);
	if (type == 1 && !ft_here_document((*tkns)->content, env, \
			ft_tkn_count(*tkns, "<<") + ft_tkn_count(*tkns, "<"), fake))
		return (0);
	if (type == 2 && !ft_redirect_input((*tkns)->content, env, \
			ft_tkn_count(*tkns, "<<") + ft_tkn_count(*tkns, "<"), fake))
		return (0);
	if (type == 3 && !ft_append_output((*tkns)->content, env, \
			ft_tkn_count(*tkns, ">>") + ft_tkn_count(*tkns, ">"), fake))
		return (0);
	if (type == 4 && !ft_redirect_output((*tkns)->content, env, \
			ft_tkn_count(*tkns, ">>") + ft_tkn_count(*tkns, ">"), fake))
		return (0);
	ft_lst_pop(tkns, &free);
	return (ft_redirect(tkns, env, fake));
}
