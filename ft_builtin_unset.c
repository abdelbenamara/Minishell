/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 04:20:23 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/11 10:32:32 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_unset(char **argv, t_list **env)
{
	size_t	i;
	char	*var;

	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
		return (ft_pstderr3("unset", argv[1], "invalid option"), 2);
	i = 1;
	while (argv[i])
	{
		var = ft_env_gets(env, argv[i]);
		if (var)
			ft_lst_pop(env, &free);
		++i;
	}
	return (EXIT_SUCCESS);
}
