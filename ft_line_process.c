/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/27 16:12:46 by abenamar         ###   ########.fr       */
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
		return (printf("syntax error near unexpected token `|'\n"), 0);
	if (str[0] != '|')
	{
		if (str[i] == '<')
			return (printf("syntax error near unexpected token `<'\n"), 0);
		if (str[i] == '>')
			return (printf("syntax error near unexpected token `>'\n"), 0);
	}
	return (1);
}

static uint8_t	ft_check_syntax(char *str)
{
	size_t	i;

	if (str[0] == '|')
		return (printf("syntax error near unexpected token `|'\n"), 0);
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
		return (printf("syntax error near unexpected token `newline'\n"), 0);
	return (1);
}

static void	ft_close_here_documents(t_list *lst)
{
	while (ft_tkn_count(lst, "<<", 1))
	{
		if (!ft_strncmp(lst->content, "<<", 3))
		{
			lst = lst->next;
			ft_close(ft_atoi(lst->content));
		}
		lst = lst->next;
	}
}

static uint8_t	ft_handle_here_documents(t_list **tkns, t_list **env)
{
	t_list	*lst;

	lst = *tkns;
	while (ft_tkn_count(lst, "<<", 1))
	{
		if (!ft_strncmp(lst->content, "<<", 3))
		{
			lst = lst->next;
			if (!ft_here_document((char **) &(lst->content), env, \
					!(ft_tkn_count(lst, "<<", 0) + ft_tkn_count(lst, "<", 0))))
				return (ft_close_here_documents(*tkns), 0);
		}
		lst = lst->next;
	}
	return (1);
}

int	ft_line_process(char **line, t_list **env)
{
	t_list	*tkns;
	int		code;
	t_list	*lst;

	if (!ft_check_syntax(*line))
		return (2);
	tkns = NULL;
	if (!ft_line_parse(*line, &tkns))
		return (ft_lstclear(&tkns, &free), EXIT_FAILURE);
	if (!tkns)
		return (EXIT_SUCCESS);
	(free(*line), *line = NULL);
	if (!ft_handle_here_documents(&tkns, env))
	{
		if (g_signum == SIGINT)
			return (ft_lstclear(&tkns, &free), 130);
		return (ft_lstclear(&tkns, &free), EXIT_FAILURE);
	}
	code = ft_pipeline(&tkns, env);
	(ft_close_here_documents(tkns), lst = tkns);
	while (lst && ft_is_redirection(lst->content))
		lst = lst->next->next;
	if (lst)
		ft_builtin(lst->content, env, 1);
	return (ft_lstclear(&tkns, &free), code);
}
