/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:54:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/08 04:49:40 by abenamar         ###   ########.fr       */
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

# ifndef FAILURE
#  define FAILURE	"\033[01;31mx"
# endif

# ifndef SUCCESS
#  define SUCCESS	"\033[01;32mo"
# endif

# ifndef PROMPT
#  define PROMPT	"\033[00m \033[01;33mminishell\033[00m:\033[01;34m"
# endif

extern int	g_signum;

int		ft_event_hook(void);

void	ft_free_tab(char **tab);

void	ft_lst_pop(t_list **lst, void (*del)(void *));

char	*ft_env_gets(t_list **env, char *key);
void	ft_env_puts(t_list **env, char *key, char *value);
void	ft_env_puti(t_list **env, char *key, int value);

/* ************************************************************************** */
/*                                                                            */
/*                                redirections                                */
/*                                                                            */
/* ************************************************************************** */

# ifndef DLINES_MAX
#  define DLINES_MAX	32
# endif

uint8_t	ft_here_document(char *limiter, int *writefd);
uint8_t	ft_redirect_input(char *filename, int *writefd);
uint8_t	ft_redirect_output(char *filename, int openflag, int *readfd);

/* ************************************************************************** */
/*                                                                            */
/*                                  builtins                                  */
/*                                                                            */
/* ************************************************************************** */

int		ft_echo(char **argv, t_list **env);
int		ft_pwd(char **argv, t_list **env);
int		ft_env(char **argv, t_list **env);
int		ft_pass(char **argv, t_list **env);

int		ft_cd(char *cmd, char **argv, t_list **env);
int		ft_export(char *cmd, char **argv, t_list **env);
int		ft_unset(char *cmd, char **argv, t_list **env);
int		ft_exit(char *cmd, char **argv, t_list **env);

/* ************************************************************************** */
/*                                                                            */
/*                                  commands                                  */
/*                                                                            */
/* ************************************************************************** */

char	*ft_parse_redirection(char c, char *cmd, t_list **cmds);

char	**ft_parse_arguments(char *cmd);

int		ft_execute_builtin(char *cmd, t_list **env);
int		ft_execute_command(char *cmd, t_list **env);

int		ft_pipe_builtin(t_list **cmds, t_list **env, int wstatus);
int		ft_pipe_command(t_list **cmds, t_list **env, int *writefd, int *readfd);

int		ft_process_line(char *line, t_list **env);

#endif
