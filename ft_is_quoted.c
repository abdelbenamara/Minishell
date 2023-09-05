/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:18:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 18:33:08 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_is_quoted(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 1;
	while (str[i + j] && str[i + j] != str[0])
		++j;
	if (str[i + j] == str[0])
		i += j;
	return (i);
}
