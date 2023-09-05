/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:18:51 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/06 00:59:01 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_realpath(t_list **env, char *filename, char *filename_path)
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
			return (free(filename_path), ft_free_tab(envpath), NULL);
		if (!access(filepath, X_OK))
			return (free(filename_path), ft_free_tab(envpath), filepath);
		free(filepath);
		++i;
	}
	return (free(filename_path), ft_free_tab(envpath), ft_strdup(filename));
}
