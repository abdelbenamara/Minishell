/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:11:01 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 05:19:52 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_variables(t_list **env)
{
	t_list	*lst;

	lst = *env;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2))
		{
			if (*(ft_strchr(((char *) lst->content), '=') + 1))
				ft_printf("%s\n", lst->content);
		}
		lst = lst->next;
	}
}

int	ft_builtin_env(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	(void) cmd;
	if (!env)
		return (EXIT_FAILURE);
	if (argv[1])
	{
		if (argv[1][0] == '-')
		{
			if (!silent)
				ft_pstderr3("env", argv[1], "invalid option");
			return (125);
		}
		if (!silent)
			ft_pstderr3("env", argv[1], "invalid argument");
		return (EXIT_FAILURE);
	}
	if (silent)
		return (EXIT_SUCCESS);
	ft_print_variables(env);
	return (EXIT_SUCCESS);
}
