/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 05:20:47 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_check_redirection(char *str)
{
	size_t	i;

	i = 1;
	if (str[0] != '|' && str[i] == str[0])
		++i;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		++i;
	if (str[i] == '|')
		return (ft_printf("syntax error near unexpected token `|'\n"), 0);
	if (str[0] != '|')
	{
		if (str[i] == '<')
			return (ft_printf("syntax error near unexpected token `<'\n"), 0);
		if (str[i] == '>')
			return (ft_printf("syntax error near unexpected token `>'\n"), 0);
	}
	return (1);
}

static uint8_t	ft_check_syntax(char *str)
{
	size_t	i;

	if (str[0] == '|')
		return (ft_printf("syntax error near unexpected token `|'\n"), 0);
	i = 0;
	while (str[i])
	{
		i += ft_is_quoted(str + i);
		if ((str[i] == '|' || str[i] == '<' || str[i] == '>')
			&& !ft_check_redirection(str + i))
			return (0);
		++i;
	}
	--i;
	if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		return (ft_printf("syntax error near unexpected token `newline'\n"), 0);
	return (1);
}

static uint8_t	ft_handle_here_documents(t_list *lst, t_list **env)
{
	while (ft_tkn_count(lst, "<<", 1))
	{
		if (!ft_strncmp(lst->content, "<<", 3))
		{
			lst = lst->next;
			if (!ft_here_document((char **) &(lst->content), env, \
					!(ft_tkn_count(lst, "<<", 0) + ft_tkn_count(lst, "<", 0))))
				return (0);
		}
		lst = lst->next;
	}
	return (1);
}

static void	ft_handle_builtin(t_list *lst, t_list **env)
{
	while (lst && ft_is_redirection(lst->content))
	{
		if (!ft_strncmp(lst->content, "<<", 3))
			close(ft_atoi(lst->next->content));
		lst = lst->next->next;
	}
	if (lst)
		ft_builtin(lst->content, env, 1);
}

int	ft_line_process(char **line, t_list **env)
{
	t_list	*tkns;
	int		code;

	if (!ft_check_syntax(*line))
		return (2);
	tkns = NULL;
	if (!ft_line_parse(*line, &tkns))
		return (ft_lstclear(&tkns, &free), EXIT_FAILURE);
	if (!tkns)
		return (EXIT_SUCCESS);
	(free(*line), *line = NULL);
	if (!ft_handle_here_documents(tkns, env))
		return (ft_lstclear(&tkns, &free), EXIT_FAILURE);
	code = ft_pipeline(&tkns, env);
	if (tkns)
		ft_handle_builtin(tkns, env);
	return (ft_lstclear(&tkns, &free), code);
}
