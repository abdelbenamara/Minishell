/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_output.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 14:33:31 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/11 09:59:05 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	ft_append_output(char *file, t_list **env, size_t rout, uint8_t fake)
{
	char	**argv;
	int		fd;

	argv = ft_command_split(file, env, ' ', 1);
	if (!argv || argv[1])
		return (ft_pstderr2(file, "ambiguous redirect"), ft_tab_free(argv), 0);
	fd = open(argv[0], O_WRONLY | O_CREAT | O_APPEND, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
		return (ft_perror(argv[0]), ft_tab_free(argv), 0);
	if (!rout && !fake && dup2(fd, STDOUT_FILENO) == -1)
		return (ft_perror2("dup2", argv[0]), 0);
	if (close(fd) == -1)
		return (ft_perror2("close", argv[0]), 0);
	return (ft_tab_free(argv), 1);
}
