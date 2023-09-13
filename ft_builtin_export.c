/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 02:22:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 05:20:12 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_shellsort(char **tab, int size)
{
	int		gap;
	char	*tmp;
	int		i;
	int		j;

	gap = 1;
	while (gap - 1 < size)
		gap *= 2;
	while (gap - 1 >= 0)
	{
		i = gap;
		while (i < size)
		{
			tmp = tab[i];
			j = i;
			while (j >= gap && ft_strncmp(tab[j - gap], tmp, SIZE_MAX) > 0)
			{
				tab[j] = tab[j - gap];
				j -= gap;
			}
			tab[j] = tmp;
			++i;
		}
		gap /= 2;
	}
}

static void	ft_print_variables(t_list **env)
{
	char	**envp;
	size_t	i;
	char	*str;

	envp = ft_env_to_tab(*env);
	ft_shellsort(envp, ft_lstsize(*env) - 1);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "_=", 2))
		{
			str = ft_strchr(envp[i], '=');
			*str = '\0';
			ft_printf("declare -x %s", envp[i]);
			if (*(str + 1))
				(ft_printf("=\"%s\"", str + 1));
			ft_printf("\n");
		}
		++i;
	}
	ft_tab_free(envp);
}

static uint8_t	ft_check_identifier(char *str, uint8_t silent)
{
	size_t	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		if (!silent)
			ft_dprintf(STDERR_FILENO, \
				"export: `%s': not a valid identifier\n", str);
		return (0);
	}
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	if (str[i] && str[i] != '=')
	{
		if (!silent)
			ft_dprintf(STDERR_FILENO, \
				"export: `%s': not a valid identifier\n", str);
		return (0);
	}
	return (1);
}

static int	ft_export_variables(char **argv, t_list **env, uint8_t silent)
{
	int		code;
	size_t	i;
	char	*str;

	code = EXIT_SUCCESS;
	i = 1;
	while (argv[i])
	{
		if (!ft_check_identifier(argv[i], silent))
			code = EXIT_FAILURE;
		else
		{
			str = ft_strchr(argv[i], '=');
			if (str)
			{
				*str = '\0';
				str += 1;
			}
			ft_env_puts(env, argv[i], str);
		}
		++i;
	}
	return (code);
}

int	ft_builtin_export(char *cmd, char **argv, t_list **env, uint8_t silent)
{
	(void) cmd;
	if (!(argv[1]))
	{
		if (!silent)
			ft_print_variables(env);
		return (EXIT_SUCCESS);
	}
	if (!ft_strncmp(argv[1], "-", 1))
	{
		if (!silent)
			ft_pstderr3("export", argv[1], "invalid option");
		return (2);
	}
	return (ft_export_variables(argv, env, silent));
}
