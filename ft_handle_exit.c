/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:14 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 18:26:29 by abenamar         ###   ########.fr       */
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
	ft_env_puti(env, "|", count);
}

int	ft_handle_exit(t_list **cmds, t_list **env, int wstatus)
{
	char	**argv;
	int		code;

	if (!(*cmds) || (ft_env_gets(env, "|") && ft_env_geti(env, "|")))
		return (wstatus);
	if (!ft_env_gets(env, "|"))
		ft_env_put_pipe_counts(cmds, env);
	if (ft_env_geti(env, "|"))
		return (wstatus);
	argv = ft_parse_arguments((*cmds)->content);
	if (!argv)
		return (-1);
	code = wstatus;
	if (!ft_strncmp("exit", argv[0], 5))
		return (code = ft_exit(argv, env), ft_free_tab(argv), code - 512);
	return (ft_free_tab(argv), code);
}
