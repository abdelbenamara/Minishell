/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 14:16:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 00:55:23 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TODO : manage to read a script and execute its commands as in minishell.c
*/
int	ft_builtin_minishell(char **argv, t_list **env)
{
	(void) env;
	if (argv[1])
	{
		ft_printf("TODO\n");
	}
	return (EXIT_SUCCESS);
}
