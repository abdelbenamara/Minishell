/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_gets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 01:55:06 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/27 14:42:13 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_env_put_lru(t_list **env, t_list *prv, t_list *lst)
{
	if (prv)
	{
		prv->next = lst->next;
		ft_lstadd_front(env, lst);
	}
}

char	*ft_env_gets(t_list **env, char *key)
{
	size_t	n;
	t_list	*lst;
	t_list	*prv;
	char	*str;

	if (!env || !key)
		return (NULL);
	n = ft_strlen(key);
	lst = *env;
	prv = NULL;
	while (lst)
	{
		str = lst->content;
		if (str && !ft_strncmp(str, key, n) && (str[n] == '=' || !(str[n])))
		{
			ft_env_put_lru(env, prv, lst);
			if (!(str[n]))
				return (str + n);
			return (str + n + 1);
		}
		prv = lst;
		lst = lst->next;
	}
	return (NULL);
}
