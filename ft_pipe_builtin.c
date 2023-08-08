/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:08:19 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 04:51:38 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pipe_builtin(t_list **cmds, t_list **env, int wstatus)
{
	static char	*builtin[] = {
		"cd", "export", "unset", "exit"
	};
	static int	(*ft_builtin[])(char *, char **, t_list **) = {
		ft_cd, ft_export, ft_unset, ft_exit
	};
	char		**argv;
	size_t		i;
	int			code;

	if (!(*cmds))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content);
	if (!argv)
		return (-1);
	i = 0;
	code = wstatus;
	while (code == wstatus && i < 4)
	{
		if (!ft_strncmp(builtin[i], argv[0], ft_strlen(builtin[i]) + 1))
			code = ft_builtin[i]((*cmds)->content, argv, env);
		++i;
	}
	return (ft_free_tab(argv), code);
}
