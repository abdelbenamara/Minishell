/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:06:00 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/15 10:48:37 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_pwd(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	char	*cwd;

	((void) cmd, (void) env);
	if (argv[1] && argv[1][0] == '-')
	{
		if (!silent)
			ft_pstderr3("pwd: ", argv[1], ": invalid option");
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (!silent)
			ft_perror("getcwd: ");
		return (EXIT_FAILURE);
	}
	if (!silent)
		if (printf("%s\n", cwd) < 0)
			return (ft_pstderr("write error"), free(cwd), EXIT_FAILURE);
	return (free(cwd), EXIT_SUCCESS);
}
