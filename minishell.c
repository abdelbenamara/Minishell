/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/05 22:01:30 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	ft_read_script(char *filename)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (ft_perror("pipe"), 0);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (ft_perror("dup2"), 0);
	if (!ft_redirect_input(filename, pipefd))
		return (0);
	if (close(pipefd[1]))
		return (ft_perror("close"), 0);
	if (close(pipefd[0]))
		return (ft_perror("close"), 0);
	return (1);
}

static t_list	*ft_init_env(char **ep)
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
	ft_env_puti(&env, "!exit", ft_env_geti(&env, "SHLVL"));
	return (env);
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

static char	*ft_handle_line(t_list **env)
{
	char	*line;
	char	*str;

	if (isatty(STDIN_FILENO))
	{
		line = ft_get_prompt(env);
		str = readline(line);
		if (!str)
			return (ft_env_puti(env, "SHLVL", ft_env_geti(env, "SHLVL") - 1), \
				ft_printf("exit\n"), g_signum = SIGTERM, free(line), NULL);
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
		ft_env_puti(env, "?", ft_process_line(&line, env));
	return (line);
}

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	char	*line;
	int		code;

	if (!ft_handle_signals())
		return (EXIT_FAILURE);
	if (ac > 1 && !ft_read_script(av[1]))
		return (EXIT_FAILURE);
	env = ft_init_env(ep);
	line = NULL;
	while (g_signum != SIGTERM || (isatty(STDIN_FILENO)
			&& ft_env_geti(&env, "SHLVL") >= ft_env_geti(&env, "!exit")))
	{
		(free(line), g_signum = 0);
		line = ft_handle_line(&env);
		if (g_signum == SIGINT)
			(ft_env_puts(&env, "?", "130"), g_signum = 0);
		if (ft_env_gets(&env, "!pipe"))
			ft_lst_pop(&env, &free);
	}
	code = ft_env_geti(&env, "?");
	return (ft_lstclear(&env, &free), rl_clear_history(), code);
}
