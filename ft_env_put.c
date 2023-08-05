/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 04:04:45 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/05 04:45:31 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_put(t_list **env, char *key, char *value)
{
	char	*str;
	char	*var;

	if (!key || !value)
		return ;
	var = ft_strjoin("=", value);
	if (!var)
		return ;
	str = ft_strjoin(key, var);
	free(var);
	if (!str)
		return ;
	var = ft_env_get(env, key);
	if (var)
		ft_lst_pop(env, &free);
	return (ft_lstadd_front(env, ft_lstnew(str)));
}
