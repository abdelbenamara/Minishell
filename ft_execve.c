/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 13:35:24 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 15:36:27 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_is_empty_command(char *cmd, t_list **env)
{
	char	**argv;
	char	*str;
	char	*tmp;

	argv = ft_command_split(cmd, env, ' ', 0);
	if (ft_is_quoted(argv[0]))
	{
		tmp = ft_expand(argv[0], env, 0, 0);
		str = ft_command_setup(tmp, '\n', '\n', 0);
		free(tmp);
		if (!str || !(*str))
			return (ft_tab_free(argv), free(str), 1);
	}
	return (ft_tab_free(argv), 0);
}

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

int	ft_execve(char *cmd, t_list **env)
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
	if ((*path || ft_is_empty_command(cmd, env)) && !ft_strchr(path, '/'))
		(ft_pstderr2(path, ": command not found"), code = 127);
	else if (*path && access(path, F_OK) == -1)
		(ft_perror(path), code = 127);
	else if (*path && access(path, X_OK) == -1)
		(ft_perror(path), code = 126);
	else if (*path && execve(path, argv, envp) == -1)
		(ft_perror(path), code = EXIT_FAILURE);
	return (ft_tab_free(envp), ft_tab_free(argv), free(path), code);
}
