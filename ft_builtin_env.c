/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:11:01 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 13:49:58 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_env(char **argv, t_list **env)
{
	t_list	*lst;

	if (!env)
		return (EXIT_FAILURE);
	if (argv[1])
	{
		if (argv[1][0] == '-')
			return (ft_pstderr3("env", argv[1], "invalid option"), 125);
		return (ft_pstderr3("env", argv[1], "invalid argument"), EXIT_FAILURE);
	}
	lst = *env;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2)
			&& ft_strncmp(lst->content, "!line=", 6))
		{
			if (*(ft_strchr(((char *) lst->content), '=') + 1))
				ft_printf("%s\n", lst->content);
		}
		lst = lst->next;
	}
	return (EXIT_SUCCESS);
}
