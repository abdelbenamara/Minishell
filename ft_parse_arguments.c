/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 23:10:30 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/31 00:47:55 by abenamar         ###   ########.fr       */
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

static char	*ft_setup_command(char *cmd, char c, uint8_t quoted)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = ft_strdup(cmd);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			j = i + 1;
			while (str[j] && str[j] != str[i])
				++j;
			if (str[i] == str[j])
				i += ft_extract_characters(str + i, c, quoted);
		}
		if (str[i] == '\t')
			str[i] = ' ';
		++i;
	}
	return (str);
}

char	**ft_parse_arguments(char *cmd, char c, uint8_t quoted)
{
	char	*str;
	char	**argv;
	size_t	i;
	size_t	j;

	str = ft_setup_command(cmd, c, quoted);
	argv = ft_split(str, c);
	if (!argv)
		return (free(str), NULL);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '\n')
				argv[i][j] = c;
			++j;
		}
		++i;
	}
	return (free(str), argv);
}
