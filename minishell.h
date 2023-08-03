/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:54:15 by abenamar          #+#    #+#             */
/*   Updated: 2023/08/03 20:24:31 by abenamar         ###   ########.fr       */
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
int		ft_execute_command(char *cmd, char **env);
int		ft_pipe_commands(t_list **cmds, char **env, int *writefd, int *readfd);
int		ft_process_line(char *line, char **env);

#endif
