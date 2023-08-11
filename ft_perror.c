/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 16:18:45 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/10 17:44:41 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_perror(char *str)
{
	return (ft_dprintf(STDERR_FILENO, \
		"minishell: %s: %s\n", str, strerror(errno)));
}
