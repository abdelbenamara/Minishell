/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_to_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 21:31:01 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 13:49:51 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_env_to_tab(t_list *lst)
{
	char	**tab;
	size_t	i;

	tab = malloc((ft_lstsize(lst) - 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2)
			&& ft_strncmp(lst->content, "!line=", 6))
		{
			tab[i] = ft_strdup(lst->content);
			++i;
		}
		lst = lst->next;
	}
	tab[i] = NULL;
	return (tab);
}
