/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:14 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/31 00:43:22 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_env_put_pipe_counts(t_list **cmds, t_list **env)
{
	int		count;
	t_list	*lst;

	count = 0;
	lst = *cmds;
	while (lst)
	{
		if (lst->content && !ft_strncmp(lst->content, "|", 2))
			++count;
		lst = lst->next;
	}
	ft_env_puti(env, "!pipe", count);
}

static uint8_t	ft_check_arguments(char **argv)
{
	size_t	i;

	if (!(argv[1]))
		return (1);
	i = (argv[1][0] == '-');
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			return (1);
		++i;
	}
	if (argv[1] && argv[2])
		return (0);
	return (1);
}

int	ft_handle_exit(t_list **cmds, t_list **env, int wstatus)
{
	char	**argv;

	if (!(*cmds) || (ft_env_gets(env, "!pipe") && ft_env_geti(env, "!pipe")))
		return (wstatus);
	if (!ft_env_gets(env, "!pipe"))
		ft_env_put_pipe_counts(cmds, env);
	if (ft_env_geti(env, "!pipe"))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content, ' ', 0);
	if (!argv)
		return (-1);
	if (!ft_strncmp("exit", argv[0], 5))
	{
		if (!ft_check_arguments(argv))
			ft_printf("exit\n");
		else if (ft_env_geti(env, "SHLVL") == ft_env_geti(env, "!exit"))
			(ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") - 1), \
				g_signum = SIGTERM);
		else
			(ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") - 1), \
				ft_printf("exit\n"));
	}
	return (ft_free_tab(argv), wstatus);
}
