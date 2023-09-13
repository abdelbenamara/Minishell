# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/30 18:50:37 by abenamar          #+#    #+#              #
#    Updated: 2023/09/13 15:20:26 by abenamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

GNL := $(CURDIR)/libft/get_next_line.o

LIBFT := $(CURDIR)/libft/libft.a

INCLUDES := -I $(CURDIR)
INCLUDES += -I $(CURDIR)/libft

LDFLAGS := -L$(CURDIR)/libft

LDLIBS := -lft
LDLIBS += -lreadline

# **************************************************************************** #
#                                                                              #
#                                    utils                                     #
#                                                                              #
# **************************************************************************** #

SRCS := ft_pstderr.c
SRCS += ft_pstderr2.c
SRCS += ft_pstderr3.c
SRCS += ft_perror.c
SRCS += ft_perror2.c

SRCS += ft_tab_free.c
SRCS += ft_prc_new.c
SRCS += ft_prc_del.c
SRCS += ft_lst_pop.c

SRCS += ft_strjoin_and_free.c
SRCS += ft_str_replace.c
SRCS += ft_is_quoted.c
SRCS += ft_tkn_count.c
SRCS += ft_close.c

# **************************************************************************** #
#                                                                              #
#                                 environment                                  #
#                                                                              #
# **************************************************************************** #

SRCS += ft_env_gets.c
SRCS += ft_env_geti.c
SRCS += ft_env_puts.c
SRCS += ft_env_puti.c

SRCS += ft_env_to_tab.c

SRCS += ft_expand.c

# **************************************************************************** #
#                                                                              #
#                                   signals                                    #
#                                                                              #
# **************************************************************************** #

SRCS += ft_signals.c

# **************************************************************************** #
#                                                                              #
#                                 redirections                                 #
#                                                                              #
# **************************************************************************** #

SRCS += ft_is_redirection.c

SRCS += ft_here_document.c
SRCS += ft_redirect_input.c
SRCS += ft_redirect_output.c

SRCS += ft_redirect.c

# **************************************************************************** #
#                                                                              #
#                                   commands                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_command_setup.c
SRCS += ft_command_split.c

# **************************************************************************** #
#                                                                              #
#                                   builtins                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_builtin_echo.c
SRCS += ft_builtin_cd.c
SRCS += ft_builtin_pwd.c
SRCS += ft_builtin_export.c
SRCS += ft_builtin_unset.c
SRCS += ft_builtin_env.c
SRCS += ft_builtin_exit.c

# **************************************************************************** #
#                                                                              #
#                                  executions                                  #
#                                                                              #
# **************************************************************************** #

SRCS += ft_execve.c
SRCS += ft_execute.c
SRCS += ft_builtin.c
SRCS += ft_pipeline.c

SRCS += ft_line_parse.c
SRCS += ft_line_process.c

SRCS += minishell.c

OBJS := $(SRCS:.c=.o)

BOBJS := $(BSRCS:.c=.o)

CC := cc

CFLAGS := -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -g3

RM := rm -f

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(NAME): $(GNL) $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

$(GNL): $(CURDIR)/libft/get_next_line.c
	$(CC) $(CFLAGS) -DBUFFER_SIZE=1 -c $< -o $@ -I $(CURDIR)/libft

$(LIBFT):
	@$(MAKE) -C $(CURDIR)/libft $(findstring bonus, $(MAKECMDGOALS))

bonus: $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all bonus
