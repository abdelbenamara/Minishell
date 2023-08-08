# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/30 18:50:37 by abenamar          #+#    #+#              #
#    Updated: 2023/08/08 19:59:46 by abenamar         ###   ########.fr        #
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

SRCS := ft_free_tab.c

SRCS += ft_lst_pop.c

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

SRCS += ft_here_document.c
SRCS += ft_redirect_input.c
SRCS += ft_redirect_output.c

# **************************************************************************** #
#                                                                              #
#                                   builtins                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_echo.c
SRCS += ft_pwd.c
SRCS += ft_env.c
SRCS += ft_exit.c
SRCS += ft_pass.c
SRCS += ft_cd.c
SRCS += ft_export.c
SRCS += ft_unset.c

# **************************************************************************** #
#                                                                              #
#                                   commands                                   #
#                                                                              #
# **************************************************************************** #

SRCS += ft_parse_redirection.c

SRCS += ft_parse_arguments.c

SRCS += ft_handle_exit.c
SRCS += ft_execute_builtin.c
SRCS += ft_execute_command.c

SRCS += ft_pipe_builtin.c
SRCS += ft_pipe_command.c

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
