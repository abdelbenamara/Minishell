/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:11:01 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 18:32:18 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **argv, t_list **env)
{
	t_list	*lst;

	if (!env)
		return (EXIT_FAILURE);
	if (argv[1])
		return (ft_dprintf(STDERR_FILENO, \
			"env: %s: invalid option / argument\n", argv[1]), 125);
	lst = *env;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2)
			&& ft_strncmp(lst->content, "|=", 2))
			ft_printf("%s\n", lst->content);
		lst = lst->next;
	}
	return (EXIT_SUCCESS);
}
