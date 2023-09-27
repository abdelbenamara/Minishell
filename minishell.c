/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/27 13:26:27 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ft_env(char **ep)
{
	t_list	*env;
	size_t	i;
	char	*var;

	env = NULL;
	i = 0;
	while (ep[i])
	{
		var = ft_strdup(ep[i]);
		if (var)
			ft_lstadd_front(&env, ft_lstnew(var));
		++i;
	}
	ft_env_puts(&env, "?", "0");
	ft_env_puts(&env, "OLDPWD", ft_env_gets(&env, "OLDPWD"));
	return (env);
}

static char	*ft_prompt(t_list **env)
{
	const char	*home = ft_env_gets(env, "HOME");
	size_t		len;
	char		*cwd;
	char		*str;
	char		*prompt;

	len = 0;
	if (home)
		len = ft_strlen(home);
	if (ft_env_gets(env, "PWD"))
		cwd = ft_strdup(ft_env_gets(env, "PWD"));
	else
		cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	if (len && !ft_strncmp(cwd, home, len))
		str = ft_strjoin("~", cwd + len);
	else
		str = ft_strdup(cwd);
	(free(cwd), cwd = ft_strjoin(str, " $ \033[00m"), free(str));
	if (ft_atoi(ft_env_gets(env, "?")))
		prompt = ft_strjoin(__FAILURE __PROMPT, cwd);
	else
		prompt = ft_strjoin(__SUCCESS __PROMPT, cwd);
	return (free(cwd), prompt);
}

static char	*ft_readline(t_list **env)
{
	char	*line;
	char	*str;

	if (isatty(STDIN_FILENO))
	{
		line = ft_prompt(env);
		str = readline(line);
		if (!str)
			return (printf("exit\n"), g_signum = SIGTERM, free(line), NULL);
	}
	else
	{
		line = get_next_line(STDIN_FILENO);
		str = ft_strtrim(line, "\n");
		if (!str)
			return (g_signum = SIGTERM, free(line), NULL);
	}
	if (*str)
		add_history(str);
	(free(line), line = ft_strtrim(str, " "), free(str));
	if (!g_signum && line && *line)
		ft_env_puti(env, "?", ft_line_process(&line, env));
	else if (g_signum == SIGINT)
		ft_env_puts(env, "?", "130");
	return (line);
}

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	char	*line;
	int		code;

	if (!ft_signals())
		return (EXIT_FAILURE);
	if (ac > 1 && !ft_redirect_input(av[1], NULL, 1))
		return (EXIT_FAILURE);
	env = ft_env(ep);
	line = NULL;
	while (g_signum != SIGTERM)
	{
		free(line);
		g_signum = 0;
		line = ft_readline(&env);
	}
	code = ft_env_geti(&env, "?");
	return (rl_clear_history(), ft_lstclear(&env, &free), code);
}
