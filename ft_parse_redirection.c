/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:01:24 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/02 17:33:17 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_add_redirection(char *cmd, t_list **cmds)
{
	size_t	i;
	char	c;
	size_t	j;

	i = 0;
	while (cmd[i] && cmd[i] == cmd[0])
		++i;
	ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, 0, i)));
	while (cmd[i] && cmd[i] == ' ')
		++i;
	c = cmd[i];
	if (!c)
		return (i);
	j = 1;
	if (c == '\'' || c == '"')
		while (cmd[i + j] && cmd[i + j] != c)
			++j;
	else
		while (cmd[i + j] && cmd[i + j] != ' ')
			++j;
	if ((c == '\'' || c == '"') && c == cmd[i + j])
		(ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, i + 1, j - 1))), ++j);
	else
		ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, i, j)));
	return (i + j);
}

char	*ft_parse_redirection(char c, char *cmd, t_list **cmds)
{
	char	*str;
	size_t	i;
	size_t	j;
	char	*tmp;

	if (!cmd)
		return (NULL);
	tmp = ft_setup_command(cmd, c, 1);
	(free(cmd), cmd = tmp);
	str = ft_strchr(cmd, c);
	while (str)
	{
		i = 0;
		while (cmd[i] && cmd[i] != str[0])
			++i;
		j = ft_add_redirection(str, cmds);
		tmp = ft_substr(cmd, 0, i);
		str = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
		(free(cmd), cmd = ft_strjoin(tmp, str), free(str), free(tmp));
		str = ft_strchr(cmd, c);
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), ft_clean_argument(tmp, c));
}
