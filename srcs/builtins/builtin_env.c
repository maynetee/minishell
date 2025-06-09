/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:23:03 by mteichma          #+#    #+#             */
/*   Updated: 2025/04/29 02:16:34 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ms_write(1, env[i], ft_strlen(env[i])) < 0
			|| ms_write(1, "\n", 1) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	builtin_env(t_data *data, char **tokens)
{
	handle_env_var(data, "_=/usr/bin/env");
	if (tokens[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(tokens[1], 2);
		ft_putendl_fd("': No such file or directory", 2);
		return (127);
	}
	if (print_env(data->envp_copy) < 0)
	{
		perror("minishell: env");
		return (1);
	}
	return (0);
}
