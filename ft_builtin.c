/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:47:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/10 13:45:02 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin(char *cmd, t_list **env, uint8_t fake)
{
	static char	*builtin[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
	};
	static int	(*ft_builtin[])(char **, t_list **) = {
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
		if (!ft_strncmp(builtin[i], argv[0], ft_strlen(builtin[i]) + 1))
		{
			if (!fake)
				return (i = ft_builtin[i](argv, env), ft_tab_free(argv), i);
			return (ft_tab_free(argv), EXIT_SUCCESS);
		}
		++i;
	}
	return (ft_tab_free(argv), 127);
}
