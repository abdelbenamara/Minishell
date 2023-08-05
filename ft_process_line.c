/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/04 16:15:45 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expand_variable(char *line, size_t *i, char **env)
{
	char	*var;
	size_t	len;
	size_t	j;

	j = 1;
	while (ft_isalnum(line[*i + j]))
		++j;
	var = ft_substr(line, *i + 1, j - 1);
	*i += j;
	if (!var)
		return (NULL);
	len = ft_strlen(var);
	j = 0;
	while (env[j] && (ft_strncmp(env[j], var, len) || env[j][len] != '='))
		++j;
	free(var);
	if (env[j])
		return (ft_strdup(env[j] + len + 1));
	return (NULL);
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

static char	*ft_real_line(char *line, char **env)
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
		str = ft_strjoin_and_free(str, ft_real_line(line + i, env));
	}
	return (str);
}

static void	ft_parse_commands(char *line, t_list **cmds)
{
	char	**strs;
	size_t	i;
	t_list	*lst;

	strs = ft_split(line, '|');
	if (!strs)
		return (free(line));
	i = 0;
	while (strs[i])
	{
		strs[i] = ft_parse_redirection('<', strs[i], cmds);
		if (!(strs[i]))
			return (free(line));
		lst = NULL;
		strs[i] = ft_parse_redirection('>', strs[i], &lst);
		if (!(strs[i]))
			return (free(line));
		if (*(strs[i]))
			ft_lstadd_back(cmds, ft_lstnew(strs[i]));
		else
			free(strs[i]);
		ft_lstadd_back(cmds, lst);
		++i;
	}
	return (free(line), free(strs));
}

int	ft_process_line(char *line, char **env)
{
	t_list	*cmds;
	int		writefd[2];
	int		readfd[2];
	int		wstatus;
	t_list	*lst;

	cmds = NULL;
	ft_parse_commands(ft_real_line(line, env), &cmds);
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