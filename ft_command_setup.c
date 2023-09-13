/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:19:43 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 05:07:54 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_save_character(char *str, char c1, char c2)
{
	size_t	i;

	i = 1;
	while (str[i] != str[0])
	{
		if (str[i] == c1)
			str[i] = c2;
		++i;
	}
	return (i);
}

static size_t	ft_handle_quotes(char *str, char c1, char c2, uint8_t quote)
{
	size_t	i;
	size_t	j;

	i = ft_save_character(str, c1, c2);
	if (quote)
		return (i + 1);
	i -= 1;
	j = 0;
	while (j < i)
	{
		str[j] = str[j + 1];
		++j;
	}
	while (str[j] && str[j + 1])
	{
		str[j] = str[j + 2];
		++j;
	}
	str[j] = '\0';
	return (i);
}

char	*ft_command_setup(char *cmd, char c1, char c2, uint8_t quote)
{
	char	*str;
	size_t	i;

	if (!cmd)
		return (NULL);
	str = ft_strdup(cmd);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\t')
			str[i] = ' ';
		else if (str[i] == '"' && ft_is_quoted(str + i))
			i += ft_handle_quotes(str + i, c1, c2, quote);
		else if (str[i] == '\'' && ft_is_quoted(str + i))
			i += ft_handle_quotes(str + i, c1, c2, quote);
		else
			++i;
	}
	return (str);
}
