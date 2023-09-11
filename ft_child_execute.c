/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 11:23:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 14:51:00 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_realpath(t_list **env, char *filename, char *filename_path)
{
	char	**envpath;
	size_t	i;
	char	*filepath;

	if (!filename)
		return (free(filename_path), ft_strdup(""));
	if (filename[0] == '/' || !filename_path)
		return (free(filename_path), ft_strdup(filename));
	envpath = ft_split(ft_env_gets(env, "PATH"), ':');
	if (!envpath)
		return (free(filename_path), ft_strdup(filename));
	i = 0;
	while (envpath[i])
	{
		filepath = ft_strjoin(envpath[i], filename_path);
		if (!filepath)
			return (free(filename_path), ft_tab_free(envpath), NULL);
		if (!access(filepath, X_OK))
			return (free(filename_path), ft_tab_free(envpath), filepath);
		free(filepath);
		++i;
	}
	return (free(filename_path), ft_tab_free(envpath), ft_strdup(filename));
}

static int	ft_execve(char *cmd, t_list **env)
{
	char	**envp;
	char	**argv;
	char	*path;
	int		code;

	envp = ft_env_to_tab(*env);
	argv = ft_command_split(cmd, env, ' ', 1);
	if (!argv)
		return (ft_tab_free(envp), ft_tab_free(argv), EXIT_FAILURE);
	path = ft_realpath(env, argv[0], ft_strjoin("/", argv[0]));
	if (!path)
		return (ft_tab_free(envp), ft_tab_free(argv), EXIT_FAILURE);
	code = EXIT_SUCCESS;
	if (!ft_strchr(path, '/'))
		(ft_pstderr2(path, "command not found"), code = 127);
	else if (access(path, F_OK) == -1)
		(ft_perror(path), code = 127);
	else if (access(path, X_OK) == -1)
		(ft_perror(path), code = 126);
	else if (execve(path, argv, envp) == -1)
		(ft_perror(path), code = EXIT_FAILURE);
	return (ft_tab_free(envp), ft_tab_free(argv), free(path), code);
}

void	ft_child_execute(t_list **prcs, t_list **tkns, t_list **env)
{
	int	code;

	if (!ft_redirect(tkns, env, 0))
		(ft_child_exit(prcs, tkns, env), exit(EXIT_FAILURE));
	if (!ft_child_signals())
		(ft_child_exit(prcs, tkns, env), exit(EXIT_FAILURE));
	if (!(*tkns))
		(ft_child_exit(prcs, tkns, env), exit(EXIT_SUCCESS));
	code = ft_builtin((*tkns)->content, env, 1);
	if (code == 127 && g_signum != SIGTERM)
		code = ft_execve((*tkns)->content, env);
	(ft_child_exit(prcs, tkns, env), exit(code));
}
