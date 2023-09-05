/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:47:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/06 00:52:53 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_builtin(char *cmd, t_list **env)
{
	static char	*builtin[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};
	static int	(*ft_builtin[])(char **, t_list **) = {
		ft_builtin_echo, ft_builtin_skip, ft_builtin_pwd, ft_builtin_export,
		ft_builtin_skip, ft_builtin_env, ft_builtin_exit
	};
	char		**argv;
	int			i;

	if (!cmd || ft_is_redirection(cmd))
		return (EXIT_SUCCESS);
	argv = ft_parse_arguments(cmd, ' ', 0);
	if (!argv)
		return (EXIT_FAILURE);
	i = 0;
	while (argv[0] && builtin[i])
	{
		if (!ft_strncmp(builtin[i], argv[0], ft_strlen(builtin[i]) + 1))
			return (i = ft_builtin[i](argv, env), ft_free_tab(argv), i);
		++i;
	}
	return (ft_free_tab(argv), 127);
}
