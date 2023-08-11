/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:08:19 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 00:59:01 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_export(char **argv, t_list **env)
{
	int		code;
	size_t	i;

	code = EXIT_SUCCESS;
	i = 1;
	while (argv[i])
	{
		if (ft_check_export_identifier(argv[i]))
		{
			if (ft_strchr(argv[i], '='))
				ft_env_puts(env, argv[i], argv[i] + ft_strlen(argv[i]) + 1);
			else
				ft_env_puts(env, argv[i], "");
		}
		else
			code = EXIT_FAILURE;
		++i;
	}
	return (code);
}

static int	ft_handle_minishell(char **argv, t_list **env)
{
	if (!(argv[1]))
	{
		ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") + 1);
		return (EXIT_SUCCESS);
	}
	return (WEXITSTATUS(ft_env_geti(env, "!wstatus")));
}

static int	ft_pipe_builtin(t_list **cmds, t_list **env, int fd, \
	int (*ft_builtin_command)(char **, t_list **))
{
	char	**argv;
	int		code;
	int		wstatus;
	pid_t	cpid;

	argv = ft_parse_arguments((*cmds)->content);
	if (!argv)
		return (-1);
	code = ft_builtin_command(argv, env);
	if (ft_env_gets(env, "!wstatus"))
		ft_lst_pop(env, &free);
	(ft_free_tab(argv), wstatus = EXIT_SUCCESS);
	cpid = fork();
	if (cpid == -1)
		return (ft_perror("fork"), -1);
	if (!cpid)
	{
		(ft_lstclear(cmds, &free), ft_lstclear(env, &free));
		if (close(fd) == -1)
			(ft_perror("close"), exit(EXIT_FAILURE));
		exit(code);
	}
	if (waitpid(cpid, &wstatus, 0) == -1)
		return (ft_perror(argv[0]), -1);
	return (wstatus);
}

int	ft_handle_builtin(t_list **cmds, t_list **env, int fd, int wstatus)
{
	static char	*builtin[] = {
		"cd", "export", "unset", "minishell", NULL
	};
	static int	(*ft_builtin[])(char **, t_list **) = {
		ft_builtin_cd, ft_handle_export, ft_builtin_unset, ft_handle_minishell
	};
	char		**argv;
	int			i;

	if (!(*cmds))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content);
	if (!argv)
		return (-1);
	(ft_env_puti(env, "!wstatus", wstatus), i = 0);
	while (builtin[i])
	{
		if (!ft_strncmp(builtin[i], argv[0], ft_strlen(builtin[i]) + 1))
			return (ft_free_tab(argv), \
				ft_pipe_builtin(cmds, env, fd, ft_builtin[i]));
		++i;
	}
	if (ft_env_gets(env, "!wstatus"))
		ft_lst_pop(env, &free);
	return (ft_free_tab(argv), wstatus);
}
