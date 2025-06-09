/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:46:53 by mteichma          #+#    #+#             */
/*   Updated: 2025/06/04 18:51:03 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	safe_print(char *line, int do_free)
{
	if (ms_write(1, line, ft_strlen(line)) < 0 || ms_write(1, "\n", 1) < 0)
	{
		if (do_free)
			free(line);
		perror("minishell: pwd");
		return (1);
	}
	if (do_free)
		free(line);
	return (0);
}

int	builtin_pwd(t_data *data)
{
	char	*cwd;

	(void)data;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("minishell: pwd"), 1);
	return (safe_print(cwd, 1));
}
