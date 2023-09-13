/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 15:04:18 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/13 15:37:44 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_redirect_input(char *file, t_list **env, size_t last)
{
	char	**argv;
	int		fd;

	argv = ft_command_split(file, env, ' ', 1);
	if (!argv || argv[1])
		return (ft_pstderr2(file, ": ambiguous redirect"), \
			ft_tab_free(argv), 0);
	fd = open(argv[0], O_RDONLY);
	if (fd == -1)
		return (ft_perror(argv[0]), ft_tab_free(argv), 0);
	if (last && dup2(fd, STDIN_FILENO) == -1)
		return (ft_perror2("dup2", argv[0]), 0);
	if (close(fd) == -1)
		return (ft_perror2("close", argv[0]), 0);
	return (ft_tab_free(argv), 1);
}
