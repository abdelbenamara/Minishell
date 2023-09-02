/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 23:10:30 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/02 17:34:05 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_parse_arguments(char *cmd, char c, uint8_t quoted)
{
	char	*str;
	char	**argv;
	size_t	i;

	str = ft_setup_command(cmd, c, quoted);
	argv = ft_split(str, c);
	if (!argv)
		return (free(str), NULL);
	i = 0;
	while (argv[i])
	{
		ft_clean_argument(argv[i], c);
		++i;
	}
	return (free(str), argv);
}
