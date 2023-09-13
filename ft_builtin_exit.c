/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 22:38:20 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 15:36:06 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_check_first_argument(char *str, uint8_t silent)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!i && (str[i] == '-' || str[0] == '+'))
			++i;
		if (!ft_isdigit(str[i]))
		{
			while (str[i] && str[i] == ' ')
				++i;
			if (!(str[i]))
				return (1);
			if (!silent && isatty(STDIN_FILENO))
				(ft_printf("exit\n"), \
					ft_pstderr3("exit: ", str, ": numeric argument required"));
			return (0);
		}
		++i;
	}
	return (1);
}

static uint8_t	ft_handle_first_argument(char *str, uint8_t silent)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		++i;
	if (!(str[i]))
	{
		if (!silent && isatty(STDIN_FILENO))
			(ft_printf("exit\n"), \
				ft_pstderr3("exit: ", str, ": numeric argument required"));
		return (0);
	}
	if (!ft_check_first_argument(str + i, silent))
		return (0);
	return (1);
}

int	ft_builtin_exit(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	int	code;

	(void) cmd;
	if (!(argv[1]))
	{
		if (!silent && isatty(STDIN_FILENO))
			ft_printf("exit\n");
		return (g_signum = SIGTERM, ft_env_geti(env, "?"));
	}
	if (!ft_handle_first_argument(argv[1], silent))
		return (g_signum = SIGTERM, 2);
	if (argv[1] && argv[2])
	{
		if (!silent && isatty(STDIN_FILENO))
			(ft_printf("exit\n"), ft_pstderr("exit: too many arguments"));
		return (EXIT_FAILURE);
	}
	code = ft_atoi(argv[1]) % 256;
	if (code < 0)
		code += 256;
	if (!silent && isatty(STDIN_FILENO))
		ft_printf("exit\n");
	return (g_signum = SIGTERM, code);
}
