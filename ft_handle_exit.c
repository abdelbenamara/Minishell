/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:14 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/12 01:19:28 by abenamar         ###   ########.fr       */
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

/*
	TODO : check the arguments of the exit command as in ft_builtin_exit
*/
int	ft_handle_exit(t_list **cmds, t_list **env, int wstatus)
{
	char	**argv;

	if (!(*cmds) || (ft_env_gets(env, "!pipe") && ft_env_geti(env, "!pipe")))
		return (wstatus);
	if (!ft_env_gets(env, "!pipe"))
		ft_env_put_pipe_counts(cmds, env);
	if (ft_env_geti(env, "!pipe"))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content);
	if (!argv)
		return (-1);
	if (!ft_strncmp("exit", argv[0], 5))
	{
		if (ft_env_geti(env, "SHLVL") == ft_env_geti(env, "!exit"))
			(ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") - 1), \
				g_signum = SIGTERM);
		else
			(ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") - 1), \
				ft_printf("exit\n"));
	}
	return (ft_free_tab(argv), wstatus);
}
