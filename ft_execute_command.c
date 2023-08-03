/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:07:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 19:54:03 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_setup_command(char *cmd)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '"') && (!i || cmd[i - 1] == ' '))
		{
			c = cmd[i];
			j = i;
			while (cmd[++j] && cmd[j] != c)
				if (cmd[j] == ' ')
					cmd[j] = '\n';
			if (cmd[j] == c)
			{
				cmd[i] = ' ';
				cmd[j] = ' ';
				i = j;
			}
		}
		++i;
	}
}

static void	ft_clean_arguments(char **args)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '\n')
				args[i][j] = ' ';
			++j;
		}
		++i;
	}
}

static void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
		(free(tab[i]), ++i);
	free(tab);
}

static char	*ft_realpath(char **env, char *filename, char *filename_path)
{
	size_t	i;
	char	**envpath;
	char	*filepath;

	if (!filename_path)
		return (ft_strdup(filename));
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		++i;
	envpath = ft_split(env[i] + 5, ':');
	if (!envpath || !filename_path)
		return (free(filename_path), ft_strdup(filename));
	i = 0;
	while (envpath[i])
	{
		filepath = ft_strjoin(envpath[i], filename_path);
		if (!filepath)
			return (free(filename_path), ft_free_tab(envpath), NULL);
		if (!access(filepath, X_OK))
			return (free(filename_path), ft_free_tab(envpath), filepath);
		free(filepath);
		++i;
	}
	return (free(filename_path), ft_free_tab(envpath), ft_strdup(filename));
}

int	ft_execute_command(char *cmd, char **env)
{
	char	**args;
	char	*path;

	ft_setup_command(cmd);
	args = ft_split(cmd, ' ');
	if (!args)
		return (EXIT_FAILURE);
	ft_clean_arguments(args);
	path = ft_realpath(env, args[0], ft_strjoin("/", args[0]));
	if (!path)
		return (ft_free_tab(args), EXIT_FAILURE);
	if (!ft_strncmp(path, ">>", 3) || !ft_strncmp(path, ">", 2))
		return (ft_free_tab(args), free(path), EXIT_SUCCESS);
	if ((ft_strncmp(path, "../", 3) && ft_strncmp(path, "./", 2)
			&& ft_strncmp(path, "/", 1))
		|| access(path, F_OK) == -1)
		return (ft_dprintf(STDERR_FILENO, "%s: command not found\n", path), \
			ft_free_tab(args), free(path), 127);
	else if (access(path, X_OK) == -1)
		return (perror(path), ft_free_tab(args), free(path), 126);
	else if (execve(path, args, env) == -1)
		return (perror(path), ft_free_tab(args), free(path), EXIT_FAILURE);
	return (ft_free_tab(args), free(path), EXIT_SUCCESS);
}
