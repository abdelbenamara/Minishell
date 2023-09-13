/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 04:20:23 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/12 23:42:41 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_unset(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	size_t	i;
	char	*var;

	(void) cmd;
	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
	{
		if (!silent)
			ft_pstderr3("unset", argv[1], "invalid option");
		return (2);
	}
	i = 1;
	while (argv[i])
	{
		var = ft_env_gets(env, argv[i]);
		if (var)
			ft_lst_pop(env, &free);
		if (!ft_strncmp(argv[i], "SHLVL", 6))
			ft_env_puts(env, "SHLVL", "0");
		++i;
	}
	return (EXIT_SUCCESS);
}
