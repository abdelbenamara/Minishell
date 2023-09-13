/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:06:00 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/12 23:31:57 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_pwd(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	char	*cwd;

	((void) cmd, (void) env);
	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
	{
		if (!silent)
			ft_dprintf(STDERR_FILENO, "pwd: %s: invalid option\n", argv[1]);
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_perror("getcwd"), EXIT_FAILURE);
	if (!silent)
		ft_printf("%s\n", cwd);
	return (free(cwd), EXIT_SUCCESS);
}
