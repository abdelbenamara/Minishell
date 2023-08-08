/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:47:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 14:51:46 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_builtin(char *cmd, t_list **env)
{
	static char	*builtin[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};
	static int	(*ft_builtin[])(char **, t_list **) = {
		ft_echo, ft_pass, ft_pwd, ft_pass, ft_pass, ft_env, ft_exit
	};
	char		**argv;
	size_t		i;
	int			code;

	if (!cmd || (!ft_strncmp(cmd, "|", 2)
			&& !ft_strncmp(cmd, ">>", 3) && !ft_strncmp(cmd, ">", 2)))
		return (EXIT_SUCCESS);
	argv = ft_parse_arguments(cmd);
	if (!argv)
		return (EXIT_FAILURE);
	i = 0;
	code = 127;
	while (code == 127 && i < 7)
	{
		if (!ft_strncmp(builtin[i], argv[0], ft_strlen(builtin[i]) + 1))
			code = ft_builtin[i](argv, env);
		++i;
	}
	return (ft_free_tab(argv), code);
}
