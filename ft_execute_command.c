/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:07:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/10 17:47:59 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*ft_realpath(t_list **env, char *filename, char *filename_path)
{
	char	**envpath;
	size_t	i;
	char	*filepath;

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
	int		code;

	envp = ft_env_to_tab(env);
	argv = ft_parse_arguments(cmd);
	if (!argv || !(argv[0]))
		return (free(envp), ft_free_tab(argv), EXIT_FAILURE);
	path = ft_realpath(env, argv[0], ft_strjoin("/", argv[0]));
	if (!path)
		return (free(envp), ft_free_tab(argv), EXIT_FAILURE);
	code = EXIT_SUCCESS;
	if (ft_strncmp(path, "../", 3) && ft_strncmp(path, "./", 2)
		&& ft_strncmp(path, "/", 1) && access(path, F_OK) == -1)
		(ft_pstderr2(path, "command not found"), code = 127);
	else if (access(path, F_OK) == -1)
		(ft_perror(path), code = 127);
	else if (access(path, X_OK) == -1)
		(ft_perror(path), code = 126);
	else if (execve(path, argv, envp) == -1)
		(ft_perror(path), code = EXIT_FAILURE);
	return (free(envp), ft_free_tab(argv), free(path), code);
}
