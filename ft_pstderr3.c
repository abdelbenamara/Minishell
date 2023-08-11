/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pstderr3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:43:54 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/11 14:16:07 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pstderr3(char *s1, char *s2, char *s3)
{
	return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s: %s\n", s1, s2, s3));
}
