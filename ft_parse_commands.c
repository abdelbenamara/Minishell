/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:58:39 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 18:14:42 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ft_reverse_rotate(t_list **lst)
{
	t_list	*tmp;

	tmp = *lst;
	if (!tmp)
		return (NULL);
	while (tmp->next && tmp->next->next)
		tmp = tmp->next;
	ft_lstadd_front(lst, tmp->next);
	tmp->next = NULL;
	if (ft_is_redirection((*lst)->content))
		return (*lst);
	return (ft_reverse_rotate(lst));
}

t_list	**ft_parse_commands(char *eline, t_list **cmds)
{
	char	**strs;
	size_t	i;
	t_list	*lst;

	strs = ft_parse_arguments(eline, '|', 1);
	if (!strs)
		return (free(eline), NULL);
	i = 0;
	while (strs[i])
	{
		if (i > 0)
			ft_lstadd_back(cmds, ft_lstnew(ft_strdup("|")));
		strs[i] = ft_parse_redirection('<', strs[i], cmds);
		lst = NULL;
		strs[i] = ft_parse_redirection('>', strs[i], &lst);
		if (!(strs[i]))
			return (free(eline), ft_free_tab(strs + i + 1), NULL);
		if (*(strs[i]))
			ft_lstadd_back(cmds, ft_lstnew(strs[i]));
		else
			free(strs[i]);
		ft_lstadd_back(cmds, ft_reverse_rotate(&lst));
		++i;
	}
	return (free(eline), free(strs), cmds);
}
