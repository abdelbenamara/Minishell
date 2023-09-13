/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:47:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/12 23:18:30 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin(char *cmd, t_list **env, uint8_t silent)
{
	static char	*builtin[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};
	static int	(*ft_builtin[])(char *, char **, t_list **, uint8_t) = {
		ft_builtin_echo, ft_builtin_cd, ft_builtin_pwd, ft_builtin_export,
		ft_builtin_unset, ft_builtin_env, ft_builtin_exit
	};
	char		**argv;
	int			i;

	argv = ft_command_split(cmd, env, ' ', 1);
	if (!argv)
		return (EXIT_FAILURE);
	i = 0;
	while (argv[0] && builtin[i])
	{
		if (!ft_strncmp(builtin[i], argv[0], 7))
			return (i = ft_builtin[i](cmd, argv, env, silent), \
				ft_tab_free(argv), i);
		++i;
	}
	return (ft_tab_free(argv), 127);
}
