/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:34:21 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/09 18:34:39 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_is_redirection(char *cmd)
{
	return (!ft_strncmp(cmd, "<<", 3) || !ft_strncmp(cmd, "<", 2)
		|| !ft_strncmp(cmd, "|", 2)
		|| !ft_strncmp(cmd, ">>", 3) || !ft_strncmp(cmd, ">", 2));
}
