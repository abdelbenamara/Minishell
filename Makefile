# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/30 18:50:37 by abenamar          #+#    #+#              #
#    Updated: 2023/09/02 17:33:38 by abenamar         ###   ########.fr        #
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

SRCS := ft_perror.c
SRCS += ft_perror2.c
SRCS += ft_pstderr.c
SRCS += ft_pstderr2.c
SRCS += ft_pstderr3.c

SRCS += ft_strjoin_and_free.c

SRCS += ft_free_tab.c

SRCS += ft_lst_pop.c

# **************************************************************************** #
#                                                                              #
#                                 environment                                  #
#                                                                              #
# **************************************************************************** #

SRCS += ft_env_gets.c
SRCS += ft_env_geti.c
SRCS += ft_env_puts.c
SRCS += ft_env_puti.c

# **************************************************************************** #
#                                                                              #
#                                   signals                                    #
#                                                                              #
# **************************************************************************** #

SRCS += ft_handle_signals.c

# **************************************************************************** #
#                                                                              #
#                                 redirections                                 #
#                                                                              #
# **************************************************************************** #

SRCS += ft_parse_redirection.c

SRCS += ft_here_document.c
SRCS += ft_redirect_input.c
SRCS += ft_redirect_output.c

SRCS += ft_is_redirection.c

# **************************************************************************** #
#                                                                              #
#                                   builtins                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_check_export_identifier.c

SRCS += ft_builtin_echo.c
SRCS += ft_builtin_pwd.c
SRCS += ft_builtin_env.c
SRCS += ft_builtin_exit.c

SRCS += ft_builtin_skip.c

SRCS += ft_builtin_cd.c
SRCS += ft_builtin_export.c
SRCS += ft_builtin_unset.c

SRCS += ft_builtin_minishell.c

# **************************************************************************** #
#                                                                              #
#                                   commands                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_setup_command.c
SRCS += ft_clean_argument.c
SRCS += ft_parse_arguments.c

SRCS += ft_execute_builtin.c
SRCS += ft_execute_command.c

SRCS += ft_handle_input.c
SRCS += ft_handle_exit.c
SRCS += ft_handle_builtin.c
SRCS += ft_handle_output.c
SRCS += ft_handle_pipe.c

SRCS += ft_process_line.c

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
