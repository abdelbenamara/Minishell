/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 02:22:42 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/31 17:04:37 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_lst_to_tab(t_list *lst)
{
	char	**tab;
	size_t	i;

	tab = malloc((ft_lstsize(lst) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (lst)
	{
		tab[i] = ft_strdup(lst->content);
		lst = lst->next;
		++i;
	}
	tab[i] = NULL;
	return (tab);
}

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

static void	ft_print_environment_variables(t_list **env)
{
	char	**tab;
	size_t	i;
	char	*str;

	tab = ft_lst_to_tab(*env);
	ft_shellsort(tab, ft_lstsize(*env));
	i = 0;
	while (tab[i])
	{
		if (ft_strncmp(tab[i], "?=", 2) && ft_strncmp(tab[i], "!pipe=", 6)
			&& ft_strncmp(tab[i], "!exit=", 6) && ft_strncmp(tab[i], "_=", 2))
		{
			str = ft_strchr(tab[i], '=');
			*str = '\0';
			ft_printf("declare -x %s", tab[i]);
			if (*(str + 1))
				(ft_printf("=\"%s\"", str + 1));
			ft_printf("\n");
		}
		++i;
	}
	ft_free_tab(tab);
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
				"export: `%s': not a valid identifier\n", argv[i]);
			code = EXIT_FAILURE;
		}
		++i;
	}
	return (code);
}
