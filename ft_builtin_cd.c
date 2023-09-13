/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 04:46:35 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 15:38:50 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_check_for_empty_argument(char *cmd)
{
	char	**argv;

	argv = ft_command_split(cmd, NULL, ' ', 0);
	if (!argv || !(argv[1]))
		return (ft_tab_free(argv), NULL);
	if (!ft_strncmp(argv[1], "\"\"", 3) || !ft_strncmp(argv[1], "''", 3))
		return (ft_tab_free(argv), "");
	return (ft_tab_free(argv), NULL);
}

static char	*ft_handle_home(t_list **env, uint8_t silent)
{
	char	*cwd;

	cwd = ft_env_gets(env, "HOME");
	if (!cwd)
	{
		if (!silent)
			ft_pstderr("cd: HOME not set");
		return (NULL);
	}
	return (cwd);
}

static char	*ft_handle_minus(t_list **env, uint8_t silent)
{
	char	*cwd;

	cwd = ft_env_gets(env, "OLDPWD");
	if (!cwd || !(*cwd))
	{
		if (!silent)
			ft_pstderr("cd: OLDPWD not set");
		return (NULL);
	}
	if (!silent)
		ft_printf("%s\n", cwd);
	return (cwd);
}

int	ft_builtin_cd(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	char	*cwd;

	if (argv[1] && argv[2])
	{
		if (!silent)
			ft_pstderr("cd: too many arguments");
		return (EXIT_FAILURE);
	}
	cwd = argv[1];
	if (!cwd)
		cwd = ft_check_for_empty_argument(cmd);
	if (!cwd)
		cwd = ft_handle_home(env, silent);
	if (cwd && !ft_strncmp(cwd, "-", 2))
		cwd = ft_handle_minus(env, silent);
	if (!cwd)
		return (EXIT_FAILURE);
	if (chdir(cwd) == -1)
		return (ft_perror2("cd", cwd), EXIT_FAILURE);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_perror("getcwd"), EXIT_FAILURE);
	ft_env_puts(env, "OLDPWD", ft_env_gets(env, "PWD"));
	ft_env_puts(env, "PWD", cwd);
	return (free(cwd), EXIT_SUCCESS);
}
