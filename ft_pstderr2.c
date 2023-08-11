/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pstderr2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:42:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/11 14:16:04 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pstderr2(char *s1, char *s2)
{
	return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", s1, s2));
}
