/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:38:20 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 04:22:55 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char *cmd, char **argv, t_list **env)
{
	size_t	i;

	i = 1;
	while (argv[i])
		++i;
	if (i > 2)
		return (ft_dprintf(STDERR_FILENO, "exit: too many arguments\n"), \
			ft_free_tab(argv), EXIT_FAILURE);
	((void) cmd, g_signum = SIGTERM);
	if (argv[1])
	{
		i = 0;
		while (argv[1][i])
		{
			if (!ft_isdigit(argv[1][i]))
			{
				ft_dprintf(STDERR_FILENO, \
					"exit: %s: numeric argument required\n", argv[1]);
				return (2 - 256);
			}
			++i;
		}
		return (ft_atoi(argv[1]) - 256);
	}
	return (ft_atoi(ft_env_gets(env, "?")) - 256);
}
