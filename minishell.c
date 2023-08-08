/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 18:29:12 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ft_init_env(char **ep)
{
	t_list	*lst;
	size_t	i;
	char	*var;

	lst = NULL;
	i = 0;
	while (ep[i])
	{
		var = ft_strdup(ep[i]);
		if (var)
			ft_lstadd_front(&lst, ft_lstnew(var));
		++i;
	}
	ft_env_puts(&lst, "?", "0");
	return (lst);
}

static char	*ft_get_prompt(t_list **env)
{
	const char	*home = ft_env_gets(env, "HOME");
	size_t		len;
	char		*cwd;
	char		*str;
	char		*prompt;

	len = 0;
	if (home)
		len = ft_strlen(home);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		str = ft_strdup("");
	else
		str = ft_strdup(cwd);
	if (len && !ft_strncmp(cwd, home, len))
		(free(str), str = ft_strjoin("~", cwd + len));
	free(cwd);
	cwd = ft_strjoin(str, " $ \033[00m");
	free(str);
	if (ft_atoi(ft_env_gets(env, "?")))
		prompt = ft_strjoin(FAILURE PROMPT, cwd);
	else
		prompt = ft_strjoin(SUCCESS PROMPT, cwd);
	free(cwd);
	return (prompt);
}

static char	*ft_handle_line(t_list **env, int *code)
{
	char	*str;
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		str = ft_get_prompt(env);
		line = readline(str);
		free(str);
	}
	else
		line = get_next_line(STDIN_FILENO);
	if (!g_signum && line && *line)
	{
		add_history(line);
		*code = ft_process_line(line, env);
		ft_env_puti(env, "?", *code);
	}
	if (g_signum == SIGTERM)
		return (ft_env_puti(env, "?", (-1 * (*code)) + 512), free(line), NULL);
	return (line);
}

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	char	*line;
	int		code;

	((void) ac, (void) av);
	ft_handle_signals();
	env = ft_init_env(ep);
	line = ft_strdup("");
	code = 0;
	while (line)
	{
		free(line);
		line = ft_handle_line(&env, &code);
		if (g_signum == SIGINT)
			(ft_env_puts(&env, "?", "130"), g_signum = 0);
		if (ft_env_gets(&env, "|"))
			ft_lst_pop(&env, &free);
	}
	code = ft_env_geti(&env, "?");
	return (ft_lstclear(&env, &free), ft_printf("exit\n"), code);
}
