/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:18:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/08 09:53:50 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_is_quoted(char *str)
{
	size_t	i;
	size_t	j;

	if (str[0] != '\'' && str[0] != '"')
		return (0);
	i = 0;
	j = 1;
	while (str[i + j] && str[i + j] != str[0])
		++j;
	if (str[i + j] == str[0])
		i += j;
	return (i);
}
