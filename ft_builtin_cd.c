/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 04:46:35 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 00:46:26 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_cd(char **argv, t_list **env)
{
	char	*cwd;

	if (argv[1] && argv[2])
		return (ft_pstderr2("cd", "too many arguments"), EXIT_FAILURE);
	cwd = argv[1];
	if (!cwd)
		cwd = ft_env_gets(env, "HOME");
	if (!cwd)
		return (ft_pstderr2("cd", "HOME not set"), EXIT_FAILURE);
	if (!ft_strncmp(cwd, "-", 2))
		cwd = ft_env_gets(env, "OLDPWD");
	if (!(*cwd))
		return (ft_pstderr2("cd", "OLDPWD not set"), EXIT_FAILURE);
	if (chdir(cwd) == -1)
		return (ft_perror2("cd", cwd), EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	ft_env_puts(env, "OLDPWD", ft_env_gets(env, "PWD"));
	ft_env_puts(env, "PWD", cwd);
	free(cwd);
	return (EXIT_SUCCESS);
}
