/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:54:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/02 17:33:10 by abenamar         ###   ########.fr       */
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

int		ft_perror(char *str);
int		ft_perror2(char *s1, char *s2);
int		ft_pstderr(char *str);
int		ft_pstderr2(char *s1, char *s2);
int		ft_pstderr3(char *s1, char *s2, char *s3);

char	*ft_strjoin_and_free(char *s1, char *s2);

void	ft_free_tab(char **tab);

void	ft_lst_pop(t_list **lst, void (*del)(void *));

/* ************************************************************************** */
/*                                                                            */
/*                                environment                                 */
/*                                                                            */
/* ************************************************************************** */

char	*ft_env_gets(t_list **env, char *key);
int		ft_env_geti(t_list **env, char *key);
void	ft_env_puts(t_list **env, char *key, char *value);
void	ft_env_puti(t_list **env, char *key, int value);

/* ************************************************************************** */
/*                                                                            */
/*                                  signals                                   */
/*                                                                            */
/* ************************************************************************** */

extern int	g_signum;

uint8_t	ft_handle_signals(void);

/* ************************************************************************** */
/*                                                                            */
/*                                redirections                                */
/*                                                                            */
/* ************************************************************************** */

# ifndef DLINES_MAX
#  define DLINES_MAX	32
# endif

char	*ft_parse_redirection(char c, char *cmd, t_list **cmds);

uint8_t	ft_here_document(char *limiter, int *writefd);
uint8_t	ft_redirect_input(char *filename, int *writefd);
uint8_t	ft_redirect_output(char *filename, int openflag, int *readfd);

uint8_t	ft_is_redirection(char *cmd);

/* ************************************************************************** */
/*                                                                            */
/*                                  builtins                                  */
/*                                                                            */
/* ************************************************************************** */

uint8_t	ft_check_export_identifier(char *str);

int		ft_builtin_echo(char **argv, t_list **env);
int		ft_builtin_pwd(char **argv, t_list **env);
int		ft_builtin_env(char **argv, t_list **env);
int		ft_builtin_exit(char **argv, t_list **env);

int		ft_builtin_skip(char **argv, t_list **env);

int		ft_builtin_cd(char **argv, t_list **env);
int		ft_builtin_export(char **argv, t_list **env);
int		ft_builtin_unset(char **argv, t_list **env);

int		ft_builtin_minishell(char **argv, t_list **env);

/* ************************************************************************** */
/*                                                                            */
/*                                  commands                                  */
/*                                                                            */
/* ************************************************************************** */

char	*ft_setup_command(char *cmd, char c, uint8_t quoted);
char	*ft_clean_argument(char *str, char c);
char	**ft_parse_arguments(char *cmd, char c, uint8_t quoted);

int		ft_execute_builtin(char *cmd, t_list **env);
int		ft_execute_command(char *cmd, t_list **env);

int		ft_handle_input(t_list **cmds, int *writefd);
int		ft_handle_exit(t_list **cmds, t_list **env, int wstatus);
int		ft_handle_builtin(t_list **cmds, t_list **env, int fd, int wstatus);
int		ft_handle_output(t_list **cmds, int *readfd, int fd, int wstatus);
int		ft_handle_pipe(t_list **cmds, t_list **env, int *writefd, int *readfd);

int		ft_process_line(char **line, t_list **env);

#endif
