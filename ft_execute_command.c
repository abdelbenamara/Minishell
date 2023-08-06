/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:07:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 18:42:38 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_setup_command(char *cmd)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			c = cmd[i];
			j = i + 1;
			while (cmd[j] && cmd[j] != c)
				++j;
			if (c == cmd[j])
			{
				cmd[i] = ' ';
				while ((++i) < j)
					if (cmd[i] == ' ')
						cmd[i] = '\n';
				cmd[j] = ' ';
			}
		}
		++i;
	}
	return (cmd);
}

static char	**ft_env_to_tab(t_list **env)
{
	t_list	*lst;
	char	**tab;
	size_t	i;

	lst = *env;
	tab = malloc(ft_lstsize(lst) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (lst)
	{
		if (ft_strncmp(lst->content, "?=", 2))
		{
			tab[i] = lst->content;
			++i;
		}
		lst = lst->next;
	}
	tab[i] = NULL;
	return (tab);
}

static void	ft_clean_arguments(char **argv)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '\n')
				argv[i][j] = ' ';
			++j;
		}
		++i;
	}
}

static char	*ft_realpath(t_list **env, char *filename, char *filename_path)
{
	char	**envpath;
	size_t	i;
	char	*filepath;

	if (!filename_path)
		return (ft_strdup(filename));
	envpath = ft_split(ft_env_get(env, "PATH"), ':');
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

int	ft_execute_command(char *cmd, t_list **env)
{
	char	**envp;
	char	**argv;
	char	*path;

	envp = ft_env_to_tab(env);
	argv = ft_split(ft_setup_command(cmd), ' ');
	if (!argv)
		return (free(envp), EXIT_FAILURE);
	ft_clean_arguments(argv);
	path = ft_realpath(env, argv[0], ft_strjoin("/", argv[0]));
	if (!path)
		return (free(envp), ft_free_tab(argv), EXIT_FAILURE);
	if ((ft_strncmp(path, "../", 3) && ft_strncmp(path, "./", 2)
			&& ft_strncmp(path, "/", 1))
		|| access(path, F_OK) == -1)
		return (ft_dprintf(STDERR_FILENO, "%s: command not found\n", path), \
			free(envp), ft_free_tab(argv), free(path), 127);
	else if (access(path, X_OK) == -1)
		return (perror(path), free(envp), ft_free_tab(argv), free(path), 126);
	else if (execve(path, argv, envp) == -1)
		return (perror(path), \
			free(envp), ft_free_tab(argv), free(path), EXIT_FAILURE);
	return (free(envp), ft_free_tab(argv), free(path), EXIT_SUCCESS);
}
