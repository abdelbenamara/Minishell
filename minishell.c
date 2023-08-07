/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/07 16:43:47 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum = 0;

static void	ft_rl_redisplay(int signum)
{
	g_signum = signum;
	if (signum == SIGINT)
		rl_done = 1;
}

static void	ft_setup_sigaction(void)
{
	struct sigaction	int_act;
	struct sigaction	quit_act;

	rl_event_hook = &ft_event_hook;
	int_act.sa_handler = &ft_rl_redisplay;
	int_act.sa_flags = 0;
	if (sigemptyset(&(int_act.sa_mask)) == -1)
		perror("SIGINT sigemptyset");
	if (sigaction(SIGINT, &int_act, NULL) == -1)
		perror("SIGINT sigaction");
	quit_act.sa_handler = SIG_IGN;
	quit_act.sa_flags = 0;
	if (sigemptyset(&(quit_act.sa_mask)) == -1)
		perror("SIGQUIT sigemptyset");
	if (sigaction(SIGQUIT, &quit_act, NULL) == -1)
		perror("SIGQUIT sigaction");
}

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
	if (!cwd)
		str = ft_strdup("");
	else
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
	ft_setup_sigaction();
	env = ft_init_env(ep);
	line = ft_strdup("");
	while (line)
	{
		g_signum = 0;
		free(line);
		prompt = ft_get_prompt(&env);
		line = readline(prompt);
		free(prompt);
		if (!g_signum && line && *line)
			(add_history(line), code = ft_itoa(ft_process_line(line, &env)), \
				ft_env_put(&env, "?", code), free(code));
		else
			ft_env_put(&env, "?", "0");
		if (g_signum == SIGINT)
			ft_env_put(&env, "?", "130");
	}
	return (ft_lstclear(&env, &free), ft_printf("exit\n"), EXIT_SUCCESS);
}
