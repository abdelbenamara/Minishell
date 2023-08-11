/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:55:04 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/09 18:30:22 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_builtin_echo(char **argv, t_list **env)
{
	uint8_t	trailing_newline;
	size_t	i;

	(void) env;
	trailing_newline = 1;
	i = 1;
	if (argv[1] && !ft_strncmp(argv[1], "-n", 3))
	{
		trailing_newline = 0;
		++i;
	}
	while (argv[i])
	{
		if ((trailing_newline && i > 1) || (!trailing_newline && i > 2))
			ft_printf(" ");
		ft_printf("%s", argv[i]);
		++i;
	}
	if (trailing_newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
