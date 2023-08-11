/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 02:22:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 00:54:31 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TODO : print all declared variables in alphabetical order
	with doubles quotes on values
*/
static void	ft_print_environment_variables(t_list **env)
{
	t_list	*lst;

	lst = *env;
	while (lst)
	{
		ft_printf("declare -x TODO\n");
		lst = lst->next;
	}
}

int	ft_builtin_export(char **argv, t_list **env)
{
	int		code;
	size_t	i;

	if (!(argv[1]))
		return (ft_print_environment_variables(env), EXIT_SUCCESS);
	if (!ft_strncmp(argv[1], "-", 1))
		return (ft_pstderr3("export", argv[1], "invalid option"), 2);
	code = EXIT_SUCCESS;
	i = 1;
	while (argv[i])
	{
		if (!ft_check_export_identifier(argv[i]))
		{
			ft_dprintf(STDERR_FILENO, \
				"export: `%s': not a valid identifier\n", argv[1]);
			code = EXIT_FAILURE;
		}
		++i;
	}
	return (code);
}
