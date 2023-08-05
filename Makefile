# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/30 18:50:37 by abenamar          #+#    #+#              #
#    Updated: 2023/08/05 04:45:06 by abenamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

LIBFT := $(CURDIR)/libft/libft.a

INCLUDES := -I $(CURDIR)
INCLUDES += -I $(CURDIR)/libft

LDFLAGS := -L$(CURDIR)/libft

LDLIBS := -lft
LDLIBS += -lreadline

SRCS := ft_free_tab.c
SRCS += ft_env_get.c
SRCS += ft_env_put.c
SRCS += ft_lst_pop.c
SRCS += ft_here_document.c
SRCS += ft_redirect_input.c
SRCS += ft_redirect_output.c
SRCS += ft_parse_redirection.c
SRCS += ft_execute_command.c
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

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

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
