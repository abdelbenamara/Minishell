/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 13:07:05 by abenamar         ###   ########.fr       */
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

static uint8_t	ft_is_builtin(t_list *lst, t_list **env)
{
	while (lst && ft_is_redirection(lst->content))
		lst = lst->next->next;
	if (lst && !ft_builtin(lst->content, env, 1))
		return (1);
	return (0);
}

static void	ft_debug(t_list *lst)
{
	ft_printf("\033[01;33m[DEBUG] tkns : [ %s", lst->content);
	while (lst->next)
		(ft_printf(", %s", lst->next->content), lst = lst->next);
	ft_printf(" ]\033[00m\n");
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
	ft_debug(tkns);
	if (!ft_tkn_count(tkns, "|") && ft_is_builtin(tkns, env))
	{
		if (!ft_redirect(&tkns, env, 1))
			return (ft_lstclear(&tkns, &free), EXIT_FAILURE);
		code = ft_builtin(tkns->content, env, 0);
		return (ft_lstclear(&tkns, &free), code);
	}
	return (ft_pipeline(&tkns, env));
}
