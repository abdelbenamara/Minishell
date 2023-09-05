/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setup_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 17:19:43 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 20:59:29 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_save_character(char *str, char c)
{
	size_t	i;

	i = 1;
	while (str[i] != str[0])
	{
		if (str[i] == c)
			str[i] = '\n';
		++i;
	}
	return (i);
}

static size_t	ft_extract_characters(char *str, char c, uint8_t quoted)
{
	char	q;
	size_t	i;
	size_t	j;

	i = ft_save_character(str, c);
	if (quoted)
		return (i);
	q = str[0];
	str[0] = str[1];
	i = 0;
	while (str[i + 1] != q)
	{
		str[i] = str[i + 1];
		++i;
	}
	j = i;
	while (str[j] && str[j + 1])
	{
		str[j] = str[j + 2];
		++j;
	}
	return (i);
}

char	*ft_setup_command(char *cmd, char c, uint8_t quoted)
{
	char	*str;
	size_t	i;

	str = ft_strdup(cmd);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\t')
			str[i] = ' ';
		else if ((str[i] == '\'' || str[i] == '"') && ft_is_quoted(str + i))
			i += ft_extract_characters(str + i, c, quoted);
		++i;
	}
	return (str);
}
