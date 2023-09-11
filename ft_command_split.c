/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 23:10:30 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/10 14:33:36 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_command_expand(char *cmd, t_list **env, char c)
{
	char	*tmp;
	char	*str;

	tmp = ft_expand(cmd, env, 0, 0);
	str = ft_command_setup(tmp, c, 0);
	free(tmp);
	return (str);
}

char	**ft_command_split(char *cmd, t_list **env, char c, uint8_t expand)
{
	char	*str;
	char	**argv;
	size_t	i;

	if (expand)
		str = ft_command_expand(cmd, env, c);
	else
		str = ft_command_setup(cmd, c, 1);
	argv = ft_split(str, c);
	if (!argv)
		return (free(str), NULL);
	i = 0;
	while (argv[i])
	{
		ft_str_replace(argv[i], '\n', c);
		if (expand)
			ft_str_replace(ft_str_replace(argv[i], -1, '\''), -2, '"');
		++i;
	}
	return (free(str), argv);
}
