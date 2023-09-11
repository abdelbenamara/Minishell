/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_to_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 21:31:01 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 18:24:42 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_env_to_tab(t_list *lst)
{
	char	**tab;
	size_t	i;

	tab = malloc(ft_lstsize(lst) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2))
		{
			tab[i] = ft_strdup(lst->content);
			++i;
		}
		lst = lst->next;
	}
	tab[i] = NULL;
	return (tab);
}
