/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:54:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/05 20:57:14 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
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
#  define PROMPT	"\033[01;33mminishell\033[00m:\033[01;34m"
# endif

void	ft_free_tab(char **tab);
char	*ft_env_get(t_list **env, char *key);
void	ft_env_put(t_list **env, char *key, char *value);
void	ft_lst_pop(t_list **lst, void (*del)(void *));

/* ************************************************************************** */
/*                                                                            */
/*                                redirections                                */
/*                                                                            */
/* ************************************************************************** */

# ifndef DLINES_MAX
#  define DLINES_MAX	32
# endif

void	ft_here_document(char *limiter, int *writefd);
void	ft_redirect_input(char *filename, int *writefd);
void	ft_redirect_output(char *filename, int openflag, int *readfd);

/* ************************************************************************** */
/*                                                                            */
/*                                  commands                                  */
/*                                                                            */
/* ************************************************************************** */

char	*ft_parse_redirection(char c, char *cmd, t_list **cmds);
int		ft_execute_command(char *cmd, t_list **env);
int		ft_pipe_command(t_list **cmds, t_list **env, int *writefd, int *readfd);
int		ft_process_line(char *line, t_list **env);

#endif
