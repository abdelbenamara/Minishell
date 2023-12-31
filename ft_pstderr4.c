/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pstderr4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenamar <abenamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 17:43:54 by abenamar          #+#    #+#             */
/*   Updated: 2023/09/15 10:51:02 by abenamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pstderr4(char *s1, char *s2, char *s3, char *s4)
{
	char	*str;
	int		n;

	str = ft_strdup("minishell: ");
	if (s1)
		str = ft_strjoin_and_free(str, ft_strdup(s1));
	if (s2)
		str = ft_strjoin_and_free(str, ft_strdup(s2));
	if (s3)
		str = ft_strjoin_and_free(str, ft_strdup(s3));
	if (s4)
		str = ft_strjoin_and_free(str, ft_strdup(s4));
	str = ft_strjoin_and_free(str, ft_strdup("\n"));
	n = ft_strlen(str);
	if (write(STDERR_FILENO, str, n * sizeof(char)) == -1)
		return (-1);
	free(str);
	return (n);
}
