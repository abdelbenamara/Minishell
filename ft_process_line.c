/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 22:35:51 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expand_variable(char *line, size_t *i, t_list **env)
{
	size_t	j;
	size_t	k;
	char	*key;
	char	*var;

	j = (*i);
	if (j > 0)
		--j;
	while (j > 0 && line[j] == ' ')
		--j;
	k = 1;
	while (ft_isalnum(line[(*i) + k]) || line[(*i) + k] == '?')
		++k;
	if ((j > 0 && !ft_strncmp(line + j - 1, "<<", 2)
			&& (j == 1 || line[j - 2] != '<'))
		|| (k == 1))
		return (++(*i), ft_strdup("$"));
	key = ft_substr(line, (*i) + 1, k - 1);
	(*i) += k;
	if (!key)
		return (NULL);
	var = ft_env_get(env, key);
	if (var)
		return (free(key), ft_strdup(var));
	return (free(key), NULL);
}

static char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (str);
}

static char	*ft_expand_line(char *line, t_list **env)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = NULL;
	i = 0;
	while (line[i] && line[i] != '$')
	{
		if (line[i] == '\'')
		{
			j = 1;
			while (line[i + j] && line[i + j] != '\'')
				++j;
			if (line[i + j] == '\'')
				i += j;
		}
		++i;
	}
	str = ft_strjoin_and_free(str, ft_substr(line, 0, i));
	if (line[i] == '$')
	{
		str = ft_strjoin_and_free(str, ft_expand_variable(line, &i, env));
		str = ft_strjoin_and_free(str, ft_expand_line(line + i, env));
	}
	return (str);
}

static uint8_t	ft_parse_commands(char *eline, t_list **cmds)
{
	char	**strs;
	size_t	i;
	t_list	*lst;

	strs = ft_split(eline, '|');
	if (!strs)
		return (free(eline), 0);
	i = 0;
	while (strs[i])
	{
		if (i > 0)
			ft_lstadd_back(cmds, ft_lstnew(ft_strdup("|")));
		strs[i] = ft_parse_redirection('<', strs[i], cmds);
		lst = NULL;
		strs[i] = ft_parse_redirection('>', strs[i], &lst);
		if (!(strs[i]))
			return (free(eline), ft_free_tab(strs + i + 1), 0);
		if (*(strs[i]))
			ft_lstadd_back(cmds, ft_lstnew(strs[i]));
		else
			free(strs[i]);
		ft_lstadd_back(cmds, lst);
		++i;
	}
	return (free(eline), free(strs), 1);
}

int	ft_process_line(char *line, t_list **env)
{
	t_list	*cmds;
	int		writefd[2];
	int		readfd[2];
	int		wstatus;
	t_list	*lst;

	cmds = NULL;
	if (!ft_parse_commands(ft_expand_line(line, env), &cmds))
		return (ft_lstclear(&cmds, &free), EXIT_FAILURE);
	if (!cmds)
		return (EXIT_SUCCESS);
	ft_printf("\033[01;35m[DEBUG] cmds : [ %s", cmds->content);
	lst = cmds->next;
	while (lst)
		(ft_printf(", %s", lst->content), lst = lst->next);
	ft_printf(" ]\033[00m\n");
	if (pipe(writefd) == -1)
		return (perror("pipe"), ft_lstclear(&cmds, &free), 1);
	while (cmds)
		wstatus = ft_pipe_command(&cmds, env, writefd, readfd);
	if (wstatus == -1)
		return (ft_lstclear(&cmds, &free), EXIT_FAILURE);
	return (WEXITSTATUS(wstatus));
}
