/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/06 20:28:06 by abenamar         ###   ########.fr       */
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
	ft_env_put(&lst, "?", "0");
	return (lst);
}

static char	*ft_get_prompt(t_list **env)
{
	const char	*home = ft_env_get(env, "HOME");
	size_t		len;
	char		*cwd;
	char		*str;
	char		*prompt;

	len = 0;
	if (home)
		len = ft_strlen(home);
	cwd = getcwd(NULL, 0);
	str = ft_strdup(cwd);
	if (len && !ft_strncmp(cwd, home, len))
		(free(str), str = ft_strjoin("~", cwd + len));
	free(cwd);
	cwd = ft_strjoin(str, " $ \033[00m");
	free(str);
	if (ft_atoi(ft_env_get(env, "?")))
		prompt = ft_strjoin(FAILURE PROMPT, cwd);
	else
		prompt = ft_strjoin(SUCCESS PROMPT, cwd);
	free(cwd);
	return (prompt);
}

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	char	*line;
	char	*code;
	char	*prompt;

	((void) ac, (void) av);
	env = ft_init_env(ep);
	line = ft_strdup("");
	while (line)
	{
		free(line);
		prompt = ft_get_prompt(&env);
		line = readline(prompt);
		free(prompt);
		if (line && *line)
			(add_history(line), code = ft_itoa(ft_process_line(line, &env)));
		else
			code = ft_itoa(0);
		ft_env_put(&env, "?", code);
		free(code);
	}
	ft_lstclear(&env, &free);
	ft_printf("exit\n");
	return (EXIT_SUCCESS);
}
