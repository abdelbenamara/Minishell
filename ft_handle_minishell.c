/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_minishell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:03:47 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/02 18:41:29 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_minishell(t_list **cmds, t_list **env, int wstatus)
{
	char	**argv;
	char	*path;

	if (!(*cmds))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content, ' ', 0);
	if (!argv)
		return (-1);
	path = ft_realpath(env, argv[0], ft_strjoin("/", argv[0]));
	if ((ft_strncmp(path, "../", 3) && ft_strncmp(path, "./", 2)
			&& ft_strncmp(path, "/", 1))
		|| ft_strncmp(ft_strrchr(path, '/') + 1, "minishell", 10)
		|| access(path, X_OK) == -1)
		return (ft_free_tab(argv), free(path), wstatus);
	if (!(argv[1]))
		return (ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") + 1), \
			ft_free_tab(argv), free(path), EXIT_SUCCESS);
	return (ft_free_tab(argv), free(path), wstatus);
}
