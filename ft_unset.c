/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 04:20:23 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 04:42:23 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char *cmd, char **argv, t_list **env)
{
	size_t	i;
	char	*var;

	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
		return (ft_dprintf(STDERR_FILENO, \
			"unset: %s: invalid option\n", argv[1]), -2);
	(void) cmd;
	i = 1;
	while (argv[i])
	{
		var = ft_env_gets(env, argv[i]);
		if (var)
			ft_lst_pop(env, &free);
		++i;
	}
	return (EXIT_SUCCESS);
}
