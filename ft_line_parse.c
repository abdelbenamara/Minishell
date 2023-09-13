/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:58:39 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 14:20:08 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_add_redirection(char *str, t_list **tkns)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] == str[0])
		++i;
	ft_lstadd_back(tkns, ft_lstnew(ft_substr(str, 0, i)));
	while (str[i] == ' ')
		++i;
	j = ft_is_quoted(str + i);
	if (!j)
		j = 1;
	while (str[i + j] && str[i + j] != ' '
		&& str[i + j] != '<' && str[i + j] != '>')
		++j;
	return (ft_lstadd_back(tkns, ft_lstnew(ft_str_replace(\
		ft_str_replace(ft_substr(str, i, j), -3, '<'), -4, '>'))), i + j);
}

static char	*ft_parse_redirection(char *cmd, size_t i, t_list **tkns)
{
	size_t	j;
	char	*str;
	char	*tmp;

	j = ft_add_redirection(cmd + i, tkns);
	str = ft_substr(cmd, 0, i);
	tmp = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
	free(cmd);
	cmd = ft_strjoin(str, tmp);
	free(str);
	free(tmp);
	return (cmd);
}

static char	*ft_search_redirections(char *cmd, t_list **tkns)
{
	char	*tmp;
	size_t	i;

	if (!cmd)
		return (NULL);
	tmp = ft_command_setup(cmd, '<', -3, 1);
	(free(cmd), cmd = ft_command_setup(tmp, '>', -4, 1), free(tmp));
	if (!cmd)
		return (NULL);
	i = 0;
	while (cmd[i] && cmd[i] != '<' && cmd[i] != '>')
		++i;
	while (cmd[i])
	{
		cmd = ft_parse_redirection(cmd, i, tkns);
		if (!cmd)
			return (NULL);
		i = 0;
		while (cmd[i] && cmd[i] != '<' && cmd[i] != '>')
			++i;
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), ft_str_replace(ft_str_replace(tmp, -3, '<'), -4, '>'));
}

t_list	**ft_line_parse(char *line, t_list **tkns)
{
	char	**strs;
	size_t	i;

	strs = ft_command_split(line, NULL, '|', 0);
	if (!strs)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		if (i > 0)
			ft_lstadd_back(tkns, ft_lstnew(ft_strdup("|")));
		strs[i] = ft_search_redirections(strs[i], tkns);
		if (!(strs[i]))
			return (ft_tab_free(strs + i + 1), NULL);
		if (*(strs[i]))
			ft_lstadd_back(tkns, ft_lstnew(strs[i]));
		else
			free(strs[i]);
		++i;
	}
	return (free(strs), tkns);
}
