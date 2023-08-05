/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 01:55:06 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/05 04:59:57 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_env_get(t_list **env, char *key)
{
	size_t	len;
	t_list	*lst;
	t_list	*prv;

	if (!key)
		return (NULL);
	len = ft_strlen(key);
	lst = *env;
	prv = NULL;
	while (lst)
	{
		if (lst->content && !ft_strncmp(lst->content, key, len)
			&& ((char *) lst->content)[len] == '=')
		{
			if (prv)
			{
				prv->next = lst->next;
				ft_lstadd_front(env, lst);
			}
			return (lst->content + len + 1);
		}
		prv = lst;
		lst = lst->next;
	}
	return (NULL);
}
