/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 16:18:45 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/11 14:18:25 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_perror2(char *s1, char *s2)
{
	return (ft_dprintf(STDERR_FILENO, \
		"minishell: %s: %s: %s\n", s1, s2, strerror(errno)));
}
