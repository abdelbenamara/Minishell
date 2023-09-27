/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_puts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 04:04:45 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/27 14:14:53 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_puts(t_list **env, char *key, char *value)
{
	char	*str;
	char	*var;

	if (!key)
		return ;
	var = ft_strjoin("=", value);
	if (var)
		str = ft_strjoin(key, var);
	else
		str = ft_strdup(key);
	free(var);
	var = ft_env_gets(env, key);
	if (var)
		ft_lst_pop(env, &free);
	ft_lstadd_front(env, ft_lstnew(str));
}
