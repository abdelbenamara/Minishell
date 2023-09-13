/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 14:00:30 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 12:49:01 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_variable(char *str, t_list **env, size_t *i, uint8_t hdoc)
{
	size_t	j;
	char	*key;
	char	*var;

	j = 1;
	if (!ft_isalpha(str[(*i) + j])
		&& str[(*i) + j] != '_' && str[(*i) + j] != '?')
		return (++(*i), ft_strdup("$"));
	while (ft_isalnum(str[(*i) + j]) || str[(*i) + j] == '_')
		++j;
	if (j == 1 && str[(*i) + j] == '?')
		++j;
	if (j == 1)
		return (++(*i), ft_strdup("$"));
	key = ft_substr(str, (*i) + 1, j - 1);
	var = ft_env_gets(env, key);
	(free(key), (*i) += j);
	if (var)
	{
		if (hdoc)
			return (ft_strdup(var));
		return (ft_str_replace(ft_str_replace(\
			ft_strdup(var), '\'', -1), '"', -2));
	}
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

char	*ft_expand(char *cmd, t_list **env, uint8_t dquoted, uint8_t hdoc)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (cmd[i] && cmd[i] != '$')
	{
		if (cmd[i] == '\'' && !dquoted && !hdoc)
			i += ft_is_quoted(cmd + i);
		else if (cmd[i] == '"' && !dquoted)
			dquoted = ft_is_quoted(cmd + i);
		else if (cmd[i] == '"' && dquoted)
			dquoted = 0;
		++i;
	}
	str = ft_strjoin_and_free(str, ft_substr(cmd, 0, i));
	if (cmd[i] == '$')
	{
		str = ft_strjoin_and_free(str, ft_get_variable(cmd, env, &i, hdoc));
		str = ft_strjoin_and_free(str, ft_expand(cmd + i, env, dquoted, hdoc));
	}
	return (str);
}
