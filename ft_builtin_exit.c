/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:38:20 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/30 13:42:38 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_exit(char **argv, t_list **env)
{
	int	i;

	if (!(argv[1]))
		return (g_signum = SIGTERM, ft_env_geti(env, "?"));
	i = (argv[1][0] == '-');
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			return (g_signum = SIGTERM, ft_pstderr3("exit", \
				argv[1], "numeric argument required"), 2);
		++i;
	}
	if (argv[1] && argv[2])
		return (ft_pstderr2("exit", "too many arguments"), EXIT_FAILURE);
	i = ft_atoi(argv[1]) % 256;
	if (i < 0)
		i += 256;
	return (g_signum = SIGTERM, i);
}
