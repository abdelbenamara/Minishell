/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 18:54:30 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_parse_commands(char *line, t_list **cmds)
{
	char	**strs;
	size_t	i;
	t_list	*lst;

	strs = ft_split(line, '|');
	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		strs[i] = ft_parse_redirection('<', strs[i], cmds);
		if (!(strs[i]))
			return ;
		lst = NULL;
		strs[i] = ft_parse_redirection('>', strs[i], &lst);
		if (!(strs[i]))
			return ;
		if (*(strs[i]))
			ft_lstadd_back(cmds, ft_lstnew(strs[i]));
		else
			free(strs[i]);
		ft_lstadd_back(cmds, lst);
		++i;
	}
	free(strs);
}

int	ft_process_line(char *line, char **env)
{
	t_list	*cmds;
	int		writefd[2];
	int		readfd[2];
	int		wstatus;
	t_list	*lst;

	cmds = NULL;
	ft_parse_commands(line, &cmds);
	if (!cmds)
		return (EXIT_SUCCESS);
	ft_printf("\033[01;33m[DEBUG] cmds : [ %s", cmds->content);
	lst = cmds->next;
	while (lst)
		(ft_printf(", %s", lst->content), lst = lst->next);
	ft_printf(" ]\033[00m\n");
	if (pipe(writefd) == -1)
		return (perror("pipe"), ft_lstclear(&cmds, &free), 1);
	wstatus = ft_pipe_commands(&cmds, env, writefd, readfd);
	return (WEXITSTATUS(wstatus));
}
