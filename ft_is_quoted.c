/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:18:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/02 20:47:44 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_is_quoted(char *str, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 1;
	while (str[i + j] && str[i + j] != c)
		++j;
	if (str[i + j] == c)
		i += j;
	return (i);
}
