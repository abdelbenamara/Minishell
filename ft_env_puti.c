/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_puti.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:54:57 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 01:59:45 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_puti(t_list **env, char *key, int value)
{
	char	*str;

	str = ft_itoa(value);
	ft_env_puts(env, key, str);
	free(str);
}
