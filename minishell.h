/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:54:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 03:52:01 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE	199309L

# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"

/* ************************************************************************** */
/*                                                                            */
/*                                   utils                                    */
/*                                                                            */
/* ************************************************************************** */

# ifndef __FAILURE
#  define __FAILURE	"\033[01;31mx"
# endif

# ifndef __SUCCESS
#  define __SUCCESS	"\033[01;32mo"
# endif

# ifndef __PROMPT
#  define __PROMPT	"\033[00m \033[01;35mminishell\033[00m:\033[01;34m"
# endif

typedef struct s_proc
{
	pid_t	pid;
	int		readfd[2];
	int		writefd[2];
}	t_proc;

int		ft_pstderr(char *str);
int		ft_pstderr2(char *s1, char *s2);
int		ft_pstderr3(char *s1, char *s2, char *s3);
int		ft_perror(char *str);
int		ft_perror2(char *s1, char *s2);

void	ft_tab_free(char **tab);
t_proc	*ft_prc_new(void);
void	ft_prc_del(void *prc);
void	ft_lst_pop(t_list **lst, void (*del)(void *));

char	*ft_str_replace(char *str, char c1, char c2);
size_t	ft_is_quoted(char *str);
size_t	ft_tkn_count(t_list *lst, char *str, uint8_t all);

/* ************************************************************************** */
/*                                                                            */
/*                                environment                                 */
/*                                                                            */
/* ************************************************************************** */

char	*ft_env_gets(t_list **env, char *key);
int		ft_env_geti(t_list **env, char *key);
void	ft_env_puts(t_list **env, char *key, char *value);
void	ft_env_puti(t_list **env, char *key, int value);

char	**ft_env_to_tab(t_list *lst);

char	*ft_expand(char *cmd, t_list **env, uint8_t dquoted, uint8_t hdoc);

/* ************************************************************************** */
/*                                                                            */
/*                                  signals                                   */
/*                                                                            */
/* ************************************************************************** */

extern int	g_signum;

uint8_t	ft_signals(void);

/* ************************************************************************** */
/*                                                                            */
/*                                redirections                                */
/*                                                                            */
/* ************************************************************************** */

uint8_t	ft_is_redirection(char *cmd);

uint8_t	ft_here_document(char **limiter, t_list **env, size_t last);
uint8_t	ft_redirect_input(char *file, t_list **env, size_t last);
uint8_t	ft_redirect_output(char *file, int oflag, t_list **env, size_t last);

uint8_t	ft_redirect(t_list **tkns, t_list **env);

/* ************************************************************************** */
/*                                                                            */
/*                                  commands                                  */
/*                                                                            */
/* ************************************************************************** */

char	*ft_command_setup(char *cmd, char c1, char c2, uint8_t quote);
char	**ft_command_split(char *cmd, t_list **env, char c, uint8_t expand);

/* ************************************************************************** */
/*                                                                            */
/*                                  builtins                                  */
/*                                                                            */
/* ************************************************************************** */

int		ft_builtin_echo(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_cd(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_pwd(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_export(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_unset(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_env(char *cmd, char **argv, t_list **env, uint8_t silent);
int		ft_builtin_exit(char *cmd, char **argv, t_list **env, uint8_t silent);

/* ************************************************************************** */
/*                                                                            */
/*                                 executions                                 */
/*                                                                            */
/* ************************************************************************** */

void	ft_execute(t_list **tkns, t_list **env);
int		ft_builtin(char *cmd, t_list **env, uint8_t silent);
int		ft_pipeline(t_list **tkns, t_list **env);

t_list	**ft_line_parse(char *line, t_list **tkns);
int		ft_line_process(char **line, t_list **env);

#endif
