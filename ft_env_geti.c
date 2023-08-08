/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_geti.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:25:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 15:19:56 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_geti(t_list **env, char *key)
{
	char	*var;

	var = ft_env_gets(env, key);
	if (!var)
		return (0);
	return (ft_atoi(var));
}
