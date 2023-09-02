/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/03 01:03:03 by abenamar         ###   ########.fr       */
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
	var = ft_env_gets(env, key);
	if (var)
		return (free(key), ft_strdup(var));
	return (free(key), NULL);
}

static char	*ft_expand_line(char *line, t_list **env, size_t dquotes)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (line[i] && line[i] != '$')
	{
		if (line[i] == '\'' && !dquotes)
			i += ft_is_quoted(line + i, '\'');
		else if (line[i] == '"' && !dquotes)
			dquotes = ft_is_quoted(line + i, '"');
		else if (line[i] == '"' && dquotes)
			dquotes = 0;
		++i;
	}
	str = ft_strjoin_and_free(str, ft_substr(line, 0, i));
	if (line[i] == '$')
	{
		str = ft_strjoin_and_free(str, ft_expand_variable(line, &i, env));
		str = ft_strjoin_and_free(str, ft_expand_line(line + i, env, dquotes));
	}
	return (str);
}

static t_list	*ft_reverse_rotate(t_list **lst)
{
	t_list	*tmp;

	tmp = *lst;
	if (!tmp)
		return (NULL);
	while (tmp->next && tmp->next->next)
		tmp = tmp->next;
	ft_lstadd_front(lst, tmp->next);
	tmp->next = NULL;
	if (ft_is_redirection((*lst)->content))
		return (*lst);
	return (ft_reverse_rotate(lst));
}

static uint8_t	ft_parse_commands(char *eline, t_list **cmds)
{
	char	**strs;
	size_t	i;
	t_list	*lst;

	strs = ft_parse_arguments(eline, '|', 1);
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
		ft_lstadd_back(cmds, ft_reverse_rotate(&lst));
		++i;
	}
	return (free(eline), free(strs), 1);
}

int	ft_process_line(char **line, t_list **env)
{
	int		wstatus;
	t_list	*cmds;
	int		writefd[2];
	int		readfd[2];

	if ((*line)[0] == '|' || (*line)[ft_strlen(*line) - 1] == '|')
		return (ft_printf("syntax error near unexpected token `|'\n"), 2);
	cmds = NULL;
	if (!ft_parse_commands(ft_expand_line(*line, env, 0), &cmds))
		return (ft_lstclear(&cmds, &free), EXIT_FAILURE);
	if (!cmds)
		return (EXIT_SUCCESS);
	if (pipe(writefd) == -1)
		return (ft_perror("pipe"), ft_lstclear(&cmds, &free), EXIT_FAILURE);
	free(*line);
	wstatus = 0;
	while (cmds && wstatus >= 0)
		wstatus = ft_handle_pipe(&cmds, env, writefd, readfd);
	*line = NULL;
	if (wstatus < 0)
		return (close(writefd[0]), close(writefd[1]), close(readfd[0]), \
			close(readfd[1]), ft_lstclear(&cmds, &free), (-1 * wstatus));
	return (WEXITSTATUS(wstatus));
}
