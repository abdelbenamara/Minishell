/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_to_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:58:39 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 12:41:22 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_extract_redirection(char c, char *str, t_list **tkns)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] && str[i] == str[0] && i < 2)
		++i;
	ft_lstadd_back(tkns, ft_lstnew(ft_substr(str, 0, i)));
	while (str[i] && str[i] == ' ')
		++i;
	j = ft_is_quoted(str + i);
	if (!j)
		j = 1;
	while (str[i + j] && str[i + j] != ' ')
		++j;
	return (ft_lstadd_back(tkns, ft_lstnew(\
		ft_str_replace(ft_substr(str, i, j), '\n', c))), i + j);
}

static char	*ft_add_redirection(char *str, char *op, char *cmd, t_list **tkns)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	j = ft_strlen(str) + 1;
	while (ft_strncmp(str, cmd + i, j))
		++i;
	j = ft_extract_redirection(op[0], str, tkns);
	tmp = ft_substr(cmd, 0, i);
	str = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
	free(cmd);
	cmd = ft_strjoin(tmp, str);
	free(tmp);
	free(str);
	return (cmd);
}

static char	*ft_search_redirection(char *op, char *cmd, t_list **tkns)
{
	char	*str;
	char	*tmp;

	if (!cmd)
		return (NULL);
	tmp = ft_command_setup(cmd, op[0], 1);
	(free(cmd), cmd = tmp);
	if (!cmd)
		return (NULL);
	str = ft_strnstr(cmd, op, ft_strlen(cmd));
	while (str)
	{
		cmd = ft_add_redirection(str, op, cmd, tkns);
		if (!cmd)
			return (NULL);
		str = ft_strnstr(cmd, op, ft_strlen(cmd));
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), ft_str_replace(tmp, '\n', op[0]));
}

t_list	**ft_line_to_tokens(char *line, t_list **tkns)
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
		strs[i] = ft_search_redirection("<<", strs[i], tkns);
		strs[i] = ft_search_redirection("<", strs[i], tkns);
		strs[i] = ft_search_redirection(">", strs[i], tkns);
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
