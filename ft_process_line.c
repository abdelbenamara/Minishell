/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 01:20:11 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 19:10:27 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expand_variable(char *line, size_t *i, t_list **env)
{
	size_t	j;
	size_t	k;
	char	*key;
	char	*var;

	j = (*i);
	if (j > 0)
		--j;
	while (j > 0 && line[j] == ' ')
		--j;
	k = 1;
	while (ft_isalnum(line[(*i) + k]) || line[(*i) + k] == '?')
		++k;
	if ((j > 0 && !ft_strncmp(line + j - 1, "<<", 2)
			&& (j == 1 || line[j - 2] != '<'))
		|| (k == 1))
		return (++(*i), ft_strdup("$"));
	key = ft_substr(line, (*i) + 1, k - 1);
	(*i) += k;
	if (!key)
		return (NULL);
	var = ft_env_gets(env, key);
	if (var)
		return (free(key), ft_strdup(var));
	return (free(key), NULL);
}

static char	*ft_expand_line(char *line, t_list **env, size_t dquotes)
{
	char	*str;
	size_t	i;

	str = NULL;
	i = 0;
	while (line[i] && line[i] != '$')
	{
		if (line[i] == '\'' && !dquotes)
			i += ft_is_quoted(line + i);
		else if (line[i] == '"' && !dquotes)
			dquotes = ft_is_quoted(line + i);
		else if (line[i] == '"' && dquotes)
			dquotes = 0;
		++i;
	}
	str = ft_strjoin_and_free(str, ft_substr(line, 0, i));
	if (line[i] == '$')
	{
		str = ft_strjoin_and_free(str, ft_expand_variable(line, &i, env));
		str = ft_strjoin_and_free(str, ft_expand_line(line + i, env, dquotes));
	}
	return (str);
}

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
		if (str[i] == '\'' || str[i] == '"')
			i += ft_is_quoted(str + i);
		else if ((str[i] == '|' || str[i] == '<' || str[i] == '>')
			&& !ft_check_redirection(str + i))
			return (0);
		++i;
	}
	--i;
	if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		return (ft_printf("syntax error near unexpected token `newline'\n"), 0);
	return (1);
}

int	ft_process_line(char **line, t_list **env)
{
	int		wstatus;
	t_list	*cmds;
	int		writefd[2];
	int		readfd[2];

	if (!ft_check_syntax(*line))
		return (2);
	cmds = NULL;
	if (!ft_parse_commands(ft_expand_line(*line, env, 0), &cmds))
		return (ft_lstclear(&cmds, &free), EXIT_FAILURE);
	if (!cmds)
		return (EXIT_SUCCESS);
	if (pipe(writefd) == -1)
		return (ft_perror("pipe"), ft_lstclear(&cmds, &free), EXIT_FAILURE);
	free(*line);
	wstatus = 0;
	while (cmds && wstatus >= 0)
		wstatus = ft_handle_pipe(&cmds, env, writefd, readfd);
	*line = NULL;
	if (wstatus < 0)
		return (close(writefd[0]), close(writefd[1]), close(readfd[0]), \
			close(readfd[1]), ft_lstclear(&cmds, &free), (-1 * wstatus));
	return (WEXITSTATUS(wstatus));
}
