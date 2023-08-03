/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 20:01:24 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 18:54:07 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_add_argument(char *cmd, size_t i, t_list **cmds)
{
	size_t	j;

	j = 0;
	while (cmd[i + j] && cmd[i + j] != ' ')
		++j;
	ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, i, j)));
	return (j);
}

static size_t	ft_add_redirection(char *cmd, t_list **cmds)
{
	size_t	i;

	i = 0;
	while (cmd[i] && cmd[i] == cmd[0])
		++i;
	ft_lstadd_back(cmds, ft_lstnew(ft_substr(cmd, 0, i)));
	while (cmd[i] && cmd[i] == ' ')
		++i;
	while (cmd[i] && cmd[i] == ' ')
		++i;
	i += ft_add_argument(cmd, i, cmds);
	return (i);
}

char	*ft_parse_redirection(char c, char *cmd, t_list **cmds)
{
	char	*str;
	size_t	i;
	size_t	j;
	char	*tmp;

	str = ft_strchr(cmd, c);
	while (str)
	{
		i = ft_add_redirection(str, cmds);
		j = 0;
		while (cmd[j] && cmd[j] != str[0])
			++j;
		str = ft_substr(cmd, i + j, ft_strlen(cmd + i + j));
		tmp = ft_substr(cmd, 0, j);
		(free(cmd), cmd = ft_strjoin(tmp, str), free(str), free(tmp));
		str = ft_strchr(cmd, c);
	}
	tmp = ft_strtrim(cmd, " ");
	return (free(cmd), tmp);
}
