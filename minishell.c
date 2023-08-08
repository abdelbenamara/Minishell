/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:52:49 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 02:32:11 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum = 0;

static void	ft_handle_signal(int signum)
{
	g_signum = signum;
	if (signum == SIGINT)
		rl_done = 1;
}

static void	ft_setup_sigaction(void)
{
	struct sigaction	act;
	struct sigaction	no_act;

	rl_event_hook = &ft_event_hook;
	act.sa_handler = &ft_handle_signal;
	act.sa_flags = 0;
	if (sigemptyset(&(act.sa_mask)) == -1)
		perror("act sigemptyset");
	if (sigaction(SIGINT, &act, NULL) == -1)
		perror("SIGINT sigaction");
	no_act.sa_handler = SIG_IGN;
	no_act.sa_flags = 0;
	if (sigemptyset(&(no_act.sa_mask)) == -1)
		perror("no_act sigemptyset");
	if (sigaction(SIGQUIT, &no_act, NULL) == -1)
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

int	main(int ac, char **av, char **ep)
{
	t_list	*env;
	char	*str;
	char	*line;
	int		code;

	((void) ac, (void) av, ft_setup_sigaction(), env = ft_init_env(ep));
	line = ft_strdup("");
	code = 0;
	while (line)
	{
		(free(line), str = ft_get_prompt(&env));
		line = readline(str);
		free(str);
		if (!g_signum && line && *line)
			(add_history(line), code = ft_process_line(line, &env), \
				ft_env_puti(&env, "?", code));
		if (g_signum == SIGTERM)
			return (ft_lstclear(&env, &free), ft_printf("exit\n"), \
				(-1 * code) + 256);
		if (g_signum == SIGINT)
			(ft_env_puts(&env, "?", "130"), g_signum = 0);
	}
	code = ft_atoi(ft_env_gets(&env, "?"));
	return (ft_lstclear(&env, &free), ft_printf("exit\n"), code);
}
