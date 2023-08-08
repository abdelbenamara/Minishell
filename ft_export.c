/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 02:22:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 04:54:49 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TODO : check when quotes on value (e.g.: export toto="hello world")
*/
static uint8_t	ft_check_identifier(char *cmd, char **argv, size_t i)
{
	size_t	j;

	(void) cmd;
	if (!ft_isalpha(argv[i][0]) && argv[i][0] != '_')
		return (0);
	j = 0;
	while (argv[i][j] && (ft_isalnum(argv[i][j]) || argv[i][0] == '_'))
		++j;
	if (argv[i][j] && argv[i][j] != '=')
		return (0);
	return (1);
}

int	ft_export(char *cmd, char **argv, t_list **env)
{
	int		code;
	size_t	i;
	char	*str;

	if (argv[1] && !ft_strncmp(argv[1], "-", 1))
		return (ft_dprintf(STDERR_FILENO, \
			"export: %s: invalid option\n", argv[1]), -2);
	code = EXIT_SUCCESS;
	i = 1;
	while (argv[i])
	{
		str = ft_strchr(argv[i], '=');
		if (!ft_check_identifier(cmd, argv, i))
			(ft_dprintf(STDERR_FILENO, \
				"export: `%s': not a valid identifier\n", argv[i]), code = -1);
		else if (str)
		{
			*str = '\0';
			++str;
			ft_env_puts(env, argv[i], str);
		}
		++i;
	}
	return (code);
}
