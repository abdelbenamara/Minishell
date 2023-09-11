/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tkn_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 00:56:23 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 09:28:23 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_tkn_count(t_list *lst, char *str)
{
	const size_t	n = ft_strlen(str) + 1;
	size_t			count;

	count = 0;
	while (lst)
	{
		if (lst->content && !ft_strncmp(lst->content, str, n))
			++count;
		lst = lst->next;
	}
	return (count);
}
