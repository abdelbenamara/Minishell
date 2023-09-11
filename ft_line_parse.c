/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:58:39 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 13:23:58 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_extract_redirection(char *str, char c, t_list **tkns)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] == c)
		++i;
	ft_lstadd_back(tkns, ft_lstnew(ft_substr(str, 0, i)));
	while (str[i] == ' ')
		++i;
	j = ft_is_quoted(str + i);
	if (!j)
		j = 1;
	while (str[i + j] && str[i + j] != ' ')
		++j;
	return (ft_lstadd_back(tkns, ft_lstnew(\
		ft_str_replace(ft_substr(str, i, j), '\n', c))), i + j);
}

static char	*ft_add_redirection(char *cmd, char *str, char c, t_list **tkns)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (cmd[i] != c)
		++i;
	j = ft_extract_redirection(str, c, tkns);
	tmp = ft_substr(cmd, 0, i);
	str = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
	free(cmd);
	cmd = ft_strjoin(tmp, str);
	free(tmp);
	free(str);
	return (cmd);
}

static char	*ft_search_redirection(char *cmd, char c, t_list **tkns)
{
	char	*str;
	char	*tmp;

	if (!cmd)
		return (NULL);
	tmp = ft_command_setup(cmd, c, 1);
	(free(cmd), cmd = tmp);
	if (!cmd)
		return (NULL);
	str = ft_strchr(cmd, c);
	while (str)
	{
		cmd = ft_add_redirection(cmd, str, c, tkns);
		if (!cmd)
			return (NULL);
		str = ft_strchr(cmd, c);
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), ft_str_replace(tmp, '\n', c));
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
		strs[i] = ft_search_redirection(strs[i], '<', tkns);
		strs[i] = ft_search_redirection(strs[i], '>', tkns);
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
