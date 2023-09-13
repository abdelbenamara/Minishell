/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 20:18:38 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 04:50:07 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_is_quoted(char *str)
{
	size_t	i;

	if (str[0] != '\'' && str[0] != '"')
		return (0);
	i = 1;
	while (str[i] && str[i] != str[0])
		++i;
	if (str[i] == str[0])
		return (i);
	return (0);
}
