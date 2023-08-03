/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 19:59:52 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_home(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "HOME=", 5))
		++i;
	if (env[i])
		return (env[i] + 5);
	return (NULL);
}

static char	*ft_get_prompt(const char *home, size_t home_len)
{
	char	*cwd;
	char	*tmp;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	tmp = NULL;
	if (home && !ft_strncmp(cwd, home, home_len))
		tmp = ft_strjoin("~", cwd + home_len);
	if (tmp)
		(free(cwd), cwd = tmp);
	tmp = ft_strjoin(cwd, " $ \033[00m");
	prompt = ft_strjoin("\033[01;35mminishell\033[00m:\033[01;34m", tmp);
	free(cwd);
	free(tmp);
	return (prompt);
}

int	main(int ac, char **av, char **env)
{
	const char	*home = ft_get_home(env);
	size_t		home_len;
	char		*line;
	char		*prompt;
	int			exit_code;

	(void) ac;
	(void) av;
	home_len = 0;
	if (home)
		home_len = ft_strlen(home);
	line = NULL;
	exit_code = 0;
	while (!line || ft_strncmp(line, "exit", 5))
	{
		free(line);
		prompt = ft_get_prompt(home, home_len);
		line = readline(prompt);
		free(prompt);
		if (line && *line)
			(add_history(line), exit_code = ft_process_line(line, env));
		ft_printf("\033[01;33m[DEBUG] exit code : %d\033[00m\n", exit_code);
	}
	free(line);
	return (0);
}
