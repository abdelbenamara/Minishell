/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:06:00 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 03:55:37 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **argv, t_list **env)
{
	char	*cwd;

	(void) env;
	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
		return (ft_dprintf(STDERR_FILENO, \
			"pwd: %s: invalid option\n", argv[1]), 2);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (EXIT_FAILURE);
	ft_printf("%s\n", cwd);
	return (free(cwd), EXIT_SUCCESS);
}
