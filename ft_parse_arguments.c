/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 23:10:30 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 01:02:10 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TODO : shift chars to the left when closed quotes have been removed
	and add parameter to choose which char to 'save' and a parameter
	to remove closes quotes or not
*/
static char	*ft_setup_command(char *cmd)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			c = cmd[i];
			j = i + 1;
			while (cmd[j] && cmd[j] != c)
				++j;
			if (c == cmd[j])
			{
				cmd[i] = ' ';
				while ((++i) < j)
					if (cmd[i] == ' ')
						cmd[i] = '\n';
				cmd[j] = ' ';
			}
		}
		++i;
	}
	return (cmd);
}

char	**ft_parse_arguments(char *cmd)
{
	char	**argv;
	size_t	i;
	size_t	j;

	argv = ft_split(ft_setup_command(cmd), ' ');
	if (!argv)
		return (NULL);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '\n')
				argv[i][j] = ' ';
			++j;
		}
		++i;
	}
	return (argv);
}
