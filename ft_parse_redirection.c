/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:01:24 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 21:09:14 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_add_redirection(char c, char *cmd, t_list **cmds)
{
	size_t	i;
	char	q;
	size_t	j;

	i = 0;
	while (cmd[i] && cmd[i] == cmd[0] && i < 2)
		++i;
	ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, 0, i)));
	while (cmd[i] && cmd[i] == ' ')
		++i;
	q = cmd[i];
	if (!q)
		return (i);
	j = 1;
	if ((q == '\'' || q == '"') && ft_is_quoted(cmd + i))
		j = ft_is_quoted(cmd + i);
	else
		while (cmd[i + j] && cmd[i + j] != ' '
			&& cmd[i + j] != '<' && cmd[i + j] != '>')
			++j;
	if ((q == '\'' || q == '"') && cmd[i + j] == q)
		return (ft_lstadd_back(cmds, ft_lstnew(\
			ft_clean_argument(ft_substr(cmd, i + 1, j - 1), c))), i + j + 1);
	return (ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, i, j))), i + j);
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
		j = ft_add_redirection(c, str, cmds);
		tmp = ft_substr(cmd, 0, i);
		str = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
		(free(cmd), cmd = ft_strjoin(tmp, str), free(str), free(tmp));
		str = ft_strchr(cmd, c);
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), ft_clean_argument(tmp, c));
}
