/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:55:04 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/12 23:10:52 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_no_new_line(char *str)
{
	size_t	i;

	if (ft_strncmp(str, "-n", 2))
		return (0);
	i = 2;
	while (str[i] && str[i] == 'n')
		++i;
	if (str[i])
		return (0);
	return (1);
}

int	ft_builtin_echo(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	uint8_t	trailing_newline;
	size_t	i;
	size_t	j;

	((void) cmd, (void) env);
	trailing_newline = 1;
	i = 1;
	while (argv[i] && ft_no_new_line(argv[i]))
	{
		trailing_newline = 0;
		++i;
	}
	j = 0;
	if (!silent)
	{
		while (argv[i + j])
		{
			if (j > 0)
				ft_printf(" ");
			(ft_printf("%s", argv[i + j]), ++j);
		}
	}
	if (!silent && trailing_newline)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
